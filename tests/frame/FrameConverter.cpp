//
// Created by ivankhripunov on 09.12.23.
//

#include <gtest/gtest.h>
#include "frame/FrameConverter.h"
#include "time/TimeConverter.h"
#include "utility/parser/BulletinParser.h"
#include "../data/earth_rotation_result.hpp" //TODO: fix path
#include "time/DutCorrection.h"

TEST(CONVERTER, GCRS_ITRS) {

    const auto tolerance = static_cast<Ballistics::scalar>(1e-4);

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

    const Ballistics::Vector3<double> gcrs1 = {6700e3, 0, 0};
    const Ballistics::Vector3<double> gcrs2 = {0, 6700e3, 0};
    const Ballistics::Vector3<double> gcrs3 = {0, 0, 6700e3};

    for (const auto &set: earthRotationResult) {

        const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> tt = Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE>::buildFromJD(
                set[0]);

        const Ballistics::Matrix3d conversionMatrix = frameConverter.conversionMatrixGCRStoITRS(tt);

        const Ballistics::Vector3d itrs1 = conversionMatrix * gcrs1;
        const Ballistics::Vector3d itrs2 = conversionMatrix * gcrs2;
        const Ballistics::Vector3d itrs3 = conversionMatrix * gcrs3;

        ASSERT_NEAR(itrs1(0), set[1], tolerance);
        ASSERT_NEAR(itrs1(1), set[2], tolerance);
        ASSERT_NEAR(itrs1(2), set[3], tolerance);

        ASSERT_NEAR(itrs2(0), set[4], tolerance);
        ASSERT_NEAR(itrs2(1), set[5], tolerance);
        ASSERT_NEAR(itrs2(2), set[6], tolerance);

        ASSERT_NEAR(itrs3(0), set[7], tolerance);
        ASSERT_NEAR(itrs3(1), set[8], tolerance);
        ASSERT_NEAR(itrs3(2), set[9], tolerance);
    }
}

TEST(CONVERTER, ITRS_GCRS) {

    const auto tolerance = static_cast<Ballistics::scalar>(1e-4);

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

    const Ballistics::Vector3<double> gcrs1 = {6700e3, 0, 0};
    const Ballistics::Vector3<double> gcrs2 = {0, 6700e3, 0};
    const Ballistics::Vector3<double> gcrs3 = {0, 0, 6700e3};

    for (const auto &set: earthRotationResult) {

        const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> tt = Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE>::buildFromJD(
                set[0]);

        const Ballistics::Matrix3d conversionMatrix = frameConverter.conversionMatrixITRStoGCRS(tt);

        const Ballistics::Vector3d itrs1 = {set[1], set[2], set[3]};
        const Ballistics::Vector3d itrs2 = {set[4], set[5], set[6]};
        const Ballistics::Vector3d itrs3 = {set[7], set[8], set[9]};

        const Ballistics::Vector3d gcrs1res = conversionMatrix * itrs1;
        const Ballistics::Vector3d gcrs2res = conversionMatrix * itrs2;
        const Ballistics::Vector3d gcrs3res = conversionMatrix * itrs3;

        ASSERT_NEAR(gcrs1res.x(), gcrs1.x(), tolerance);
        ASSERT_NEAR(gcrs1res.y(), gcrs1.z(), tolerance);
        ASSERT_NEAR(gcrs1res.z(), gcrs1.z(), tolerance);

        ASSERT_NEAR(gcrs2res.x(), gcrs2.x(), tolerance);
        ASSERT_NEAR(gcrs2res.y(), gcrs2.y(), tolerance);
        ASSERT_NEAR(gcrs2res.z(), gcrs2.z(), tolerance);

        ASSERT_NEAR(gcrs3res.x(), gcrs3.x(), tolerance);
        ASSERT_NEAR(gcrs3res.y(), gcrs3.y(), tolerance);
        ASSERT_NEAR(gcrs3res.z(), gcrs3.z(), tolerance);
    }
}