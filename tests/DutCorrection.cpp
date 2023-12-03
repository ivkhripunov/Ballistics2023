//
// Created by ivankhripunov on 01.12.23.
//

#include <gtest/gtest.h>
#include <iostream>
#include "time/DutCorrection.h"

using scalar = Ballistics::scalar;

template<Ballistics::TimeModule::TimeScale Scale>
using Time = Ballistics::TimeModule::Time<Scale>;
using DutContainer = Ballistics::TimeModule::DutContainer;

using TimeScale = Ballistics::TimeModule::TimeScale;


TEST(INTERPOLATOR, MID_POINTS) {

    const Ballistics::Containers::vector<scalar> dutValues = {
            -0.11244,
            -0.11378,
            -0.11512,
            -0.11637,
            -0.11743,
            -0.11836,
            -0.11908,
            -0.11961,
            -0.12001,
            -0.12037,
            -0.12073};

    const scalar timeStartMJD_UTC = 58480;
    const scalar timeEndMJD_UTC = 58490;

    Ballistics::Containers::vector<scalar> timePointsMJD_UTC(dutValues.size());

    for (Ballistics::indexType i = 0; i < dutValues.size(); ++i) {
        timePointsMJD_UTC[i] = timeStartMJD_UTC + static_cast<scalar>(i);
    }

    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[0], timeStartMJD_UTC);
    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[timePointsMJD_UTC.size() - 1], timeEndMJD_UTC);

    const DutContainer dutContainer(timePointsMJD_UTC, dutValues);

    //mid-points test
    for (Ballistics::indexType i = 0; i < dutValues.size() - 1; ++i) {

        const scalar referenceInterpolatedDut = (dutValues[i] + dutValues[i + 1]) / 2;

        const scalar midPointMJD_UTC = (timePointsMJD_UTC[i] + timePointsMJD_UTC[i + 1]) / 2;
        const Time<Ballistics::TimeModule::TimeScale::UTC_SCALE> utc = Time<Ballistics::TimeModule::TimeScale::UTC_SCALE>::buildFromMJD(
                midPointMJD_UTC);

        ASSERT_DOUBLE_EQ(utc.mjd(), midPointMJD_UTC);

        const scalar dut = dutContainer.dut(utc);

        ASSERT_DOUBLE_EQ(dut, referenceInterpolatedDut);
    }
}

TEST(INTERPOLATOR, LEFT_POINTS) {

    const Ballistics::Containers::vector<scalar> dutValues = {
            -0.11244,
            -0.11378,
            -0.11512,
            -0.11637,
            -0.11743,
            -0.11836,
            -0.11908,
            -0.11961,
            -0.12001,
            -0.12037,
            -0.12073};

    const scalar timeStartMJD_UTC = 58480;
    const scalar timeEndMJD_UTC = 58490;

    Ballistics::Containers::vector<scalar> timePointsMJD_UTC(dutValues.size());

    for (Ballistics::indexType i = 0; i < dutValues.size(); ++i) {
        timePointsMJD_UTC[i] = timeStartMJD_UTC + static_cast<scalar>(i);
    }

    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[0], timeStartMJD_UTC);
    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[timePointsMJD_UTC.size() - 1], timeEndMJD_UTC);

    const DutContainer dutContainer(timePointsMJD_UTC, dutValues);

    //left-points test
    for (Ballistics::indexType i = 0; i < dutValues.size() - 1; ++i) {

        const scalar referenceInterpolatedDut = dutValues[i];

        const scalar midPointMJD_UTC = timePointsMJD_UTC[i];
        const Time<Ballistics::TimeModule::TimeScale::UTC_SCALE> utc = Time<Ballistics::TimeModule::TimeScale::UTC_SCALE>::buildFromMJD(
                midPointMJD_UTC);

        ASSERT_DOUBLE_EQ(utc.mjd(), midPointMJD_UTC);

        const scalar dut = dutContainer.dut(utc);

        ASSERT_DOUBLE_EQ(dut, referenceInterpolatedDut);
    }
}

TEST(INTERPOLATOR, RIGHT_POINTS) {

    const Ballistics::Containers::vector<scalar> dutValues = {
            -0.11244,
            -0.11378,
            -0.11512,
            -0.11637,
            -0.11743,
            -0.11836,
            -0.11908,
            -0.11961,
            -0.12001,
            -0.12037,
            -0.12073};

    const scalar timeStartMJD_UTC = 58480;
    const scalar timeEndMJD_UTC = 58490;

    Ballistics::Containers::vector<scalar> timePointsMJD_UTC(dutValues.size());

    for (Ballistics::indexType i = 0; i < dutValues.size(); ++i) {
        timePointsMJD_UTC[i] = timeStartMJD_UTC + static_cast<scalar>(i);
    }

    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[0], timeStartMJD_UTC);
    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[timePointsMJD_UTC.size() - 1], timeEndMJD_UTC);

    const DutContainer dutContainer(timePointsMJD_UTC, dutValues);

    //right-points test
    for (Ballistics::indexType i = 0; i < dutValues.size() - 1; ++i) {

        const scalar referenceInterpolatedDut = dutValues[i + 1];

        const scalar midPointMJD_UTC = timePointsMJD_UTC[i + 1];
        const Time<Ballistics::TimeModule::TimeScale::UTC_SCALE> utc = Time<Ballistics::TimeModule::TimeScale::UTC_SCALE>::buildFromMJD(
                midPointMJD_UTC);

        ASSERT_DOUBLE_EQ(utc.mjd(), midPointMJD_UTC);

        const scalar dut = dutContainer.dut(utc);

        ASSERT_DOUBLE_EQ(dut, referenceInterpolatedDut);
    }
}
