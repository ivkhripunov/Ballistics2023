//
// Created by ivankhripunov on 18.12.23.
//

#include <gtest/gtest.h>

#include "utility/parser/BulletinParser.h"

#include "time/DutCorrection.h"
#include "time/TimeConverter.h"

#include "frame/PolarMotionContainer.h"
#include "frame/FrameConverter.h"

#include "ephemeris/EphemerisCalculator.h"

#include "forces/Gravity/CelestialGravity.h"
#include "forces/Gravity/EarthGravity.h"

#include "atmosphere/GOST4401_81.h"
#include "forces/AtmosphereDrag/AtmosphereDrag.h"

#include "solar/IndependentShadow.h"
#include "solar/ShadowModel.h"
#include "solar/SolarPressure.h"
#include "solar/SolarIrradiance.h"
#include "forces/SolarForce/SolarForce.h"

#include "forces/AccelerationCalculator.h"

TEST(ACCELERATION, ALL) {

    const auto begin = static_cast<Ballistics::scalar>(0);
    const auto end = static_cast<Ballistics::scalar>(100000);
    const Ballistics::Containers::string path = "/home/ivankhripunov/CLionProjects/ballistics2023/data/frame/earth_rotation.csv";
    const Ballistics::indexType mjdColumnIndex = 3;
    const Ballistics::indexType xpColumnIndex = 4;
    const Ballistics::indexType ypColumnIndex = 5;
    const Ballistics::indexType dutColumnIndex = 6;

    Ballistics::Containers::vector<double> mjdData = Ballistics::Utility::getColumn(path, mjdColumnIndex,
                                                                                    begin, end);

    Ballistics::Containers::vector<double> xpData = Ballistics::Utility::getColumn(path, xpColumnIndex,
                                                                                   begin, end);

    Ballistics::Containers::vector<double> ypData = Ballistics::Utility::getColumn(path, ypColumnIndex,
                                                                                   begin, end);

    Ballistics::Containers::vector<double> dutData = Ballistics::Utility::getColumn(path, dutColumnIndex,
                                                                                    begin, end);

    const Ballistics::FrameModule::PolarMotionContainer polarMotionContainer(mjdData, xpData, ypData);
    const Ballistics::TimeModule::DutContainer dutContainer(mjdData, dutData);


    const Ballistics::TimeModule::TimeConverter<Ballistics::TimeModule::DutContainer> timeConverter(dutContainer);


    const Ballistics::FrameModule::FrameConverter<Ballistics::FrameModule::PolarMotionContainer,
            Ballistics::TimeModule::DutContainer> frameConverter(
            polarMotionContainer, timeConverter);


    const Ballistics::Containers::string ephemerisPath = "/home/ivankhripunov/CLionProjects/ballistics2023/data/ephemeris/de405.bin";
    const Ballistics::Ephemeris::EphemerisCalculator ephemerisCalculator(ephemerisPath);


    const Ballistics::Force::EarthGravityForce earthGravity("/home/ivankhripunov/CLionProjects/ballistics2023/data/earthGravity/",
                                                            "egm96", 4, 4);


    const Ballistics::Force::CelestialGravity celestialGravity;

    Ballistics::Containers::vector<Ballistics::Utility::Interpolator<Ballistics::scalar, Ballistics::scalar>::XY> xy(
            Ballistics::Atmosphere::GOST4401_81Raw::N);

    for (Ballistics::indexType i = 0; i < Ballistics::Atmosphere::GOST4401_81Raw::N; ++i) {
        xy[i] = {Ballistics::Atmosphere::GOST4401_81Raw::height[i],
                 Ballistics::Atmosphere::GOST4401_81Raw::density[i]};
    }

    const Ballistics::Utility::Interpolator<Ballistics::scalar, Ballistics::scalar> interpolator(xy);

    const Ballistics::Atmosphere::DensityCalculator densityCalculator(interpolator);

    const Ballistics::Force::AtmosphereDrag atmosphereDrag(densityCalculator);

    /******************************************************/

    const double sunRadius = 696340e3;
    const double earthRadius = 6371e3;
    const double moonRadius = 1737.4e3;

    const Ballistics::Solar::ConstantTSI tsiModel;

    const Ballistics::Solar::PenumbraModel earthShadow(sunRadius, earthRadius);
    const Ballistics::Solar::PenumbraModel moonShadow(sunRadius, moonRadius);

    const Ballistics::Solar::IndependentShadow independentShadow(earthShadow, moonShadow);

    const Ballistics::Solar::SolarPressureCalculator solarPressureCalculator(independentShadow, tsiModel);
    const Ballistics::Force::SolarForce solarForce(solarPressureCalculator);

    auto forcesTuple = std::tuple(atmosphereDrag, celestialGravity, solarForce);

    const Ballistics::Force::AccelerationCalculator accelerationCalculator(timeConverter, frameConverter,
                                                                           ephemerisCalculator, earthGravity,
                                                                           forcesTuple);


    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> timeTT(2458119, 0.5);
    const Ballistics::Vector3d position = {6800e3, 0, 0};
    const Ballistics::Vector3d velocity = {0, std::sqrt(398600e9 / 6800e3), 0};
    const double mass = 1;

    decltype(accelerationCalculator)::allSatParams allSatParams;
    allSatParams.dragCoeff_ = 1;
    allSatParams.dragArea_ = 1;
    allSatParams.solarArea = 1;

    const Ballistics::Vector3d acceleration = accelerationCalculator.calcAcceleration(earthGravity, forcesTuple,
                                                                                      timeTT,
                                                                                      position,
                                                                                      velocity,
                                                                                      mass,
                                                                                      allSatParams);

    std::cout << acceleration;
}

TEST(ACCELERATION, GRAVITY) {

    const auto begin = static_cast<Ballistics::scalar>(0);
    const auto end = static_cast<Ballistics::scalar>(100000);
    const Ballistics::Containers::string path = "/home/ivankhripunov/CLionProjects/ballistics2023/data/frame/earth_rotation.csv";
    const Ballistics::indexType mjdColumnIndex = 3;
    const Ballistics::indexType xpColumnIndex = 4;
    const Ballistics::indexType ypColumnIndex = 5;
    const Ballistics::indexType dutColumnIndex = 6;

    Ballistics::Containers::vector<double> mjdData = Ballistics::Utility::getColumn(path, mjdColumnIndex,
                                                                                    begin, end);

    Ballistics::Containers::vector<double> xpData = Ballistics::Utility::getColumn(path, xpColumnIndex,
                                                                                   begin, end);

    Ballistics::Containers::vector<double> ypData = Ballistics::Utility::getColumn(path, ypColumnIndex,
                                                                                   begin, end);

    Ballistics::Containers::vector<double> dutData = Ballistics::Utility::getColumn(path, dutColumnIndex,
                                                                                    begin, end);

    const Ballistics::FrameModule::PolarMotionContainer polarMotionContainer(mjdData, xpData, ypData);
    const Ballistics::TimeModule::DutContainer dutContainer(mjdData, dutData);


    const Ballistics::TimeModule::TimeConverter<Ballistics::TimeModule::DutContainer> timeConverter(dutContainer);


    const Ballistics::FrameModule::FrameConverter<Ballistics::FrameModule::PolarMotionContainer,
            Ballistics::TimeModule::DutContainer> frameConverter(
            polarMotionContainer, timeConverter);


    const Ballistics::Containers::string ephemerisPath = "/home/ivankhripunov/CLionProjects/ballistics2023/data/ephemeris/de405.bin";
    const Ballistics::Ephemeris::EphemerisCalculator ephemerisCalculator(ephemerisPath);


    const Ballistics::Force::EarthGravityForce earthGravity("/home/ivankhripunov/CLionProjects/ballistics2023/data/earthGravity",
                                                            "egm96", 4, 4);


    const Ballistics::Force::CelestialGravity celestialGravity;

    Ballistics::Containers::vector<Ballistics::Utility::Interpolator<Ballistics::scalar, Ballistics::scalar>::XY> xy(
            Ballistics::Atmosphere::GOST4401_81Raw::N);

    for (Ballistics::indexType i = 0; i < Ballistics::Atmosphere::GOST4401_81Raw::N; ++i) {
        xy[i] = {Ballistics::Atmosphere::GOST4401_81Raw::height[i],
                 Ballistics::Atmosphere::GOST4401_81Raw::density[i]};
    }

    const Ballistics::Utility::Interpolator<Ballistics::scalar, Ballistics::scalar> interpolator(xy);

    const Ballistics::Atmosphere::DensityCalculator densityCalculator(interpolator);

    const Ballistics::Force::AtmosphereDrag atmosphereDrag(densityCalculator);

    /******************************************************/

    const double sunRadius = 696340e3;
    const double earthRadius = 6371e3;
    const double moonRadius = 1737.4e3;

    const Ballistics::Solar::ConstantTSI tsiModel;

    const Ballistics::Solar::PenumbraModel earthShadow(sunRadius, earthRadius);
    const Ballistics::Solar::PenumbraModel moonShadow(sunRadius, moonRadius);

    const Ballistics::Solar::IndependentShadow independentShadow(earthShadow, moonShadow);

    const Ballistics::Solar::SolarPressureCalculator solarPressureCalculator(independentShadow, tsiModel);
    const Ballistics::Force::SolarForce solarForce(solarPressureCalculator);

    auto forcesTuple = std::tuple(celestialGravity);

    const Ballistics::Force::AccelerationCalculator accelerationCalculator(timeConverter, frameConverter,
                                                                           ephemerisCalculator, earthGravity,
                                                                           forcesTuple);


    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> timeTT(2458119, 0.5);
    const Ballistics::Vector3d position = {6800e3, 0, 0};
    const Ballistics::Vector3d velocity = {0, std::sqrt(398600e9 / 6800e3), 0};
    const double mass = 1;

    decltype(accelerationCalculator)::allSatParams allSatParams;

    const Ballistics::Vector3d acceleration = accelerationCalculator.calcAcceleration(earthGravity, forcesTuple,
                                                                                      timeTT,
                                                                                      position,
                                                                                      velocity,
                                                                                      mass,
                                                                                      allSatParams);

    std::cout << acceleration;
}