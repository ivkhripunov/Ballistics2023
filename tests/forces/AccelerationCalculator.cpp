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

#include "forces/AccelerationCalculator.h"

TEST(ACCELERATION, GRAVITY) {

    const auto begin = static_cast<Ballistics::scalar>(0);
    const auto end = static_cast<Ballistics::scalar>(100000);
    const Ballistics::Containers::string path = "/home/ivankhripunov/CLionProjects/ballistics2023/data/earth_rotation.csv";
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


    const Ballistics::Containers::string ephemerisPath = "/home/ivankhripunov/CLionProjects/ballistics2023/data/de405.bin";
    const Ballistics::Ephemeris::EphemerisCalculator ephemerisCalculator(ephemerisPath);


    const Ballistics::Force::EarthGravityForce earthGravity("/home/ivankhripunov/CLionProjects/ballistics2023/data/",
                                                            "egm96", 4, 4);


    const Ballistics::Force::CelestialGravity celestialGravity;


    auto forcesTuple = std::tuple(celestialGravity);

    const Ballistics::Force::AccelerationCalculator accelerationCalculator(timeConverter, frameConverter,
                                                                           ephemerisCalculator, earthGravity,
                                                                           forcesTuple);


    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> timeTT(0, 0);
    const Ballistics::Vector3d position = {42e6, 0, 0};
    const Ballistics::Vector3d velocity = Ballistics::Vector3d::Zero();
    const double mass = 1;
    decltype(accelerationCalculator)::SatParams satParams;

    const Ballistics::Vector3d acceleration = accelerationCalculator.calcAcceleration(earthGravity, forcesTuple,
                                                                                      timeTT,
                                                                                      position,
                                                                                      velocity,
                                                                                      mass,
                                                                                      satParams);
}
