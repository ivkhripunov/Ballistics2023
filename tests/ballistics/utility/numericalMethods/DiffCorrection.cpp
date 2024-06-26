//
// Created by ivankhripunov on 18.04.24.
//


#include <gtest/gtest.h>

#include "time/Time.h"
#include "utility/numericalMethods/RungeKutta.h"
#include "utility/RHS/CentralGravity.h"
#include "utility/RHS/EarthGravity.h"
#include "utility/RHS/RV.h"

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

#include "forces/AccelerationCalculator.h"

#include "fstream"
#include "utility/numericalMethods/DiffCorrection.h"

#include "utility/Noise/GaussNoise.h"

TEST(DIFF_CORRECTION, SET1) {

    const std::string currentFile = __FILE__;

    const auto begin = static_cast<Ballistics::scalar>(0);
    const auto end = static_cast<Ballistics::scalar>(100000);

    const std::string csvPath = currentFile.substr(0, currentFile.size() - 60) + "data/frame/earth_rotation.csv";

    const Ballistics::indexType mjdColumnIndex = 3;
    const Ballistics::indexType xpColumnIndex = 4;
    const Ballistics::indexType ypColumnIndex = 5;
    const Ballistics::indexType dutColumnIndex = 6;

    Ballistics::Containers::vector<double> mjdData = Ballistics::Utility::getColumn(csvPath, mjdColumnIndex,
                                                                                    begin, end);

    Ballistics::Containers::vector<double> xpData = Ballistics::Utility::getColumn(csvPath, xpColumnIndex,
                                                                                   begin, end);

    Ballistics::Containers::vector<double> ypData = Ballistics::Utility::getColumn(csvPath, ypColumnIndex,
                                                                                   begin, end);

    Ballistics::Containers::vector<double> dutData = Ballistics::Utility::getColumn(csvPath, dutColumnIndex,
                                                                                    begin, end);

    const Ballistics::FrameModule::PolarMotionContainer polarMotionContainer(mjdData, xpData, ypData);
    const Ballistics::TimeModule::DutContainer dutContainer(mjdData, dutData);


    const Ballistics::TimeModule::TimeConverter<Ballistics::TimeModule::DutContainer> timeConverter(dutContainer);


    const Ballistics::FrameModule::FrameConverter<Ballistics::FrameModule::PolarMotionContainer,
            Ballistics::TimeModule::DutContainer> frameConverter(
            polarMotionContainer, timeConverter);


    const std::string ephemerisPath = currentFile.substr(0, currentFile.size() - 60) + "data/ephemeris/de405.bin";
    //const Ballistics::Containers::string ephemerisPath = "/home/ivankhripunov/CLionProjects/ballistics2023/data/ephemeris/de405.bin";
    const Ballistics::Ephemeris::EphemerisCalculator ephemerisCalculator(ephemerisPath);


    const std::string earthPath = currentFile.substr(0, currentFile.size() - 60) + "data/earthGravity";
    Ballistics::Force::EarthGravityForce earthGravity(earthPath,
                                                      "egm96", 4, 4);

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

    auto forcesTuple = std::tuple(atmosphereDrag);

    const double mass = 1;

    const Ballistics::Force::AccelerationCalculator accelerationCalculator(timeConverter, frameConverter,
                                                                           ephemerisCalculator, earthGravity,
                                                                           forcesTuple);

    decltype(accelerationCalculator)::allSatParams allSatParams;
    allSatParams.dragCoeff_ = 0.1;
    allSatParams.dragArea_ = 1;

    const Ballistics::RHS::RV RVcalculator(accelerationCalculator);

    const Ballistics::scalar gravParameter = 3.986e14;
    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> timeTT(0, 0);
    const Ballistics::scalar initialRho = 6.7e6;
    const Ballistics::Vector3d position = {initialRho, 0, 0};
    const Ballistics::Vector3d velocity = {0, std::sqrt(gravParameter / position.norm()), 0};
    decltype(RVcalculator)::IntegrationState initialState = {
            {initialRho, 0, 0, 0, std::sqrt(gravParameter / position.norm()), 0}, timeTT};

    const Ballistics::scalar T = 2 * std::numbers::pi * initialRho * std::sqrt(initialRho / gravParameter);

    const Ballistics::scalar step = 10;

    const auto endTimeTT = timeTT + T;

    const auto integratedResult = Ballistics::NumericalMethods::integrate<Ballistics::NumericalMethods::RK4>(
            RVcalculator, initialState, endTimeTT, step, allSatParams, mass);

    EXPECT_EQ(endTimeTT.jd(), integratedResult.back().argument.jd());

    /*********************************/

    const Ballistics::scalar sigmaX = 10;
    const Ballistics::scalar sigmaXsqr = sigmaX * sigmaX;
    const Ballistics::scalar sigmaVx = 1;
    const Ballistics::scalar sigmaVxSqr = sigmaVx * sigmaVx;

    const Eigen::DiagonalMatrix<Ballistics::scalar, 6> W(1 / sigmaXsqr, 1 / sigmaXsqr, 1 / sigmaXsqr, 1 / sigmaVxSqr,
                                                         1 / sigmaVxSqr, 1 / sigmaVxSqr);

    const decltype(RVcalculator)::IntegrationState diffCorrState = {
            Ballistics::noiseRVState(initialState.vector, sigmaX, sigmaVx), initialState.argument};

    for (Ballistics::indexType count = 10; count < 25; count += 1) {
        const Ballistics::indexType measurementCount = count;
        Ballistics::Containers::vector<Ballistics::Vector<Ballistics::scalar, 6>> measurements(measurementCount);
        Ballistics::Containers::vector<Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE>> measurementsTime(
                measurementCount);
        const Ballistics::indexType indexStep = 10;//integratedResult.size() / measurementCount;
        for (Ballistics::indexType i = 0; i < measurementCount; ++i) {
            assert(i * indexStep + 10 < integratedResult.size());
            measurements[i] = integratedResult[i * indexStep + 10].vector;
            measurementsTime[i] = integratedResult[i * indexStep + 10].argument;
        }

        const auto noisedMeasurements = Ballistics::noiseRVStates(measurements, sigmaX, sigmaVx);

        const Ballistics::Containers::array<Ballistics::scalar, 6> delta = {1e-1, 1e-1, 1e-1, 1e-3, 1e-3, 1e-3};

        /*****************************************/

        const Ballistics::scalar tolerance = 1e-8;
        const Ballistics::indexType maxIteration = 100;

        const auto result = Ballistics::NumericalMethods::diffCorrection(W, diffCorrState, measurements, delta,
                                                                         measurementsTime, RVcalculator, step / 2,
                                                                         allSatParams,
                                                                         mass, tolerance, maxIteration);
        EXPECT_TRUE(result.converged);

        std::cout << (initialState.vector.segment<3>(0) - diffCorrState.vector.segment<3>(0)).norm() << ", "
                  << std::endl;

        std::cout << (initialState.vector.segment<3>(0) - result.integrationState.vector.segment<3>(0)).norm() << ", "
                  << std::endl;
    }

}
