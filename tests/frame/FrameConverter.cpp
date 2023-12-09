//
// Created by ivankhripunov on 09.12.23.
//

#include <gtest/gtest.h>
#include "frame/FrameConverter.h"
#include "time/TimeConverter.h"
#include "utility/parser/BulletinParser.h"
#include "../data/earth_rotation_result.hpp"
#include "time/DutCorrection.h"

//TODO: fix parsers

TEST(CONVERTER, SET1) {

    const auto tolerance = static_cast<Ballistics::scalar>(1e-15);

    const auto begin = static_cast<Ballistics::scalar>(0);
    const auto end = static_cast<Ballistics::scalar>(100000);
    const Ballistics::Containers::string path = "/home/ivankhripunov/CLionProjects/ballistics2023/data/earth_rotation.csv";
    const Ballistics::indexType mjdColumnIndex = 4;
    const Ballistics::indexType xpColumnIndex = 5;
    const Ballistics::indexType ypColumnIndex = 6;

    Ballistics::Containers::vector<double> mjdData = Ballistics::Utility::getColumn(path, mjdColumnIndex,
                                                                                    begin, end);

    Ballistics::Containers::vector<double> xpData = Ballistics::Utility::getColumn(path, xpColumnIndex,
                                                                                   begin, end);

    Ballistics::Containers::vector<double> ypData = Ballistics::Utility::getColumn(path, ypColumnIndex,
                                                                                   begin, end);

    Ballistics::FrameModule::PolarMotionContainer polarMotionContainer(mjdData, xpData, ypData);

    const Ballistics::Utility::MJD_DUT mjd_dut = Ballistics::Utility::CSVgetMJD_DUT(path, begin, end);

    const Ballistics::TimeModule::DutContainer dutContainer(mjd_dut.mjdVector, mjd_dut.dutVector);
    const Ballistics::TimeModule::TimeConverter<Ballistics::TimeModule::DutContainer> timeConverter(dutContainer);

    const Ballistics::FrameModule::FrameConverter<Ballistics::FrameModule::PolarMotionContainer,
            Ballistics::TimeModule::DutContainer> frameConverter(
            polarMotionContainer, timeConverter);

    const Ballistics::Vector3<double> gcrs1 = {6700e3, 0, 0};
    const Ballistics::Vector3<double> gcrs2 = {0, 6700e3, 0};
    const Ballistics::Vector3<double> gcrs3 = {0, 0, 6700e3};

    for (const auto &set: earthRotationResult) {
        const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::UTC_SCALE> utc = Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::UTC_SCALE>::buildFromMJD(
                set[0]);
        const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> tt = timeConverter.convert<Ballistics::TimeModule::TimeScale::TT_SCALE>(utc);

        const Ballistics::Matrix3d conversionMatrix = frameConverter.convertGCRStoITRS(tt);

//        const Ballistics::Vector3d itrs1 = conversionMatrix * gcrs1;
//        const Ballistics::Vector3d itrs2 = conversionMatrix * gcrs2;
//        const Ballistics::Vector3d itrs3 = conversionMatrix * gcrs3;
//
//        ASSERT_NEAR(itrs1(0), set[1], tolerance);
//        ASSERT_NEAR(itrs1(1), set[2], tolerance);
//        ASSERT_NEAR(itrs1(2), set[3], tolerance);
//
//        ASSERT_NEAR(itrs2(0), set[4], tolerance);
//        ASSERT_NEAR(itrs2(1), set[5], tolerance);
//        ASSERT_NEAR(itrs2(2), set[6], tolerance);
//
//        ASSERT_NEAR(itrs3(0), set[7], tolerance);
//        ASSERT_NEAR(itrs3(1), set[8], tolerance);
//        ASSERT_NEAR(itrs3(2), set[9], tolerance);
    }
}