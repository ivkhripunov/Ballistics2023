//
// Created by ivankhripunov on 10.12.23.
//

#include <gtest/gtest.h>
#include <iostream>
#include "frame/PolarMotionContainer.h"

using scalar = Ballistics::scalar;

template<Ballistics::TimeModule::TimeScale Scale>
using Time = Ballistics::TimeModule::Time<Scale>;

using TimeScale = Ballistics::TimeModule::TimeScale;


TEST(INTERPOLATOR, MID_POINTS) {

    const Ballistics::Containers::vector<scalar> xpValues = {
            0.095386,
            0.093289,
            0.090829,
            0.088414,
            0.086360,
            0.084450,
            0.081750,
            0.079016,
            0.076411,
            0.074159,
            0.072162};

    const Ballistics::Containers::vector<scalar> ypValues = {
            0.268935,
            0.269801,
            0.270475,
            0.270763,
            0.271107,
            0.272019,
            0.272569,
            0.272577,
            0.272536,
            0.272895,
            0.273364
    };

    const scalar timeStartMJD_UTC = 58480;
    const scalar timeEndMJD_UTC = 58490;

    Ballistics::Containers::vector<scalar> timePointsMJD_UTC(xpValues.size());

    for (Ballistics::indexType i = 0; i < xpValues.size(); ++i) {
        timePointsMJD_UTC[i] = timeStartMJD_UTC + static_cast<scalar>(i);
    }

    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[0], timeStartMJD_UTC);
    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[timePointsMJD_UTC.size() - 1], timeEndMJD_UTC);

    const Ballistics::FrameModule::PolarMotionContainer polarMotionContainer(timePointsMJD_UTC, xpValues, ypValues);

    //mid-points test
    for (Ballistics::indexType i = 0; i < xpValues.size() - 1; ++i) {

        const scalar referenceInterpolatedXp = (xpValues[i] + xpValues[i + 1]) / 2;
        const scalar referenceInterpolatedYp = (ypValues[i] + ypValues[i + 1]) / 2;

        const scalar midPointMJD_UTC = (timePointsMJD_UTC[i] + timePointsMJD_UTC[i + 1]) / 2;
        const Time<Ballistics::TimeModule::TimeScale::UTC_SCALE> utc = Time<Ballistics::TimeModule::TimeScale::UTC_SCALE>::buildFromMJD(
                midPointMJD_UTC);

        ASSERT_DOUBLE_EQ(utc.mjd(), midPointMJD_UTC);

        const Ballistics::FrameModule::PolarMotion polarMotion = polarMotionContainer.getPolarMotion(utc);

        ASSERT_DOUBLE_EQ(polarMotion.xp_, referenceInterpolatedXp);
        ASSERT_DOUBLE_EQ(polarMotion.yp_, referenceInterpolatedYp);
    }
}

TEST(INTERPOLATOR, LEFT_POINTS) {

    const Ballistics::Containers::vector<scalar> xpValues = {
            0.095386,
            0.093289,
            0.090829,
            0.088414,
            0.086360,
            0.084450,
            0.081750,
            0.079016,
            0.076411,
            0.074159,
            0.072162};

    const Ballistics::Containers::vector<scalar> ypValues = {
            0.268935,
            0.269801,
            0.270475,
            0.270763,
            0.271107,
            0.272019,
            0.272569,
            0.272577,
            0.272536,
            0.272895,
            0.273364
    };

    const scalar timeStartMJD_UTC = 58480;
    const scalar timeEndMJD_UTC = 58490;

    Ballistics::Containers::vector<scalar> timePointsMJD_UTC(xpValues.size());

    for (Ballistics::indexType i = 0; i < xpValues.size(); ++i) {
        timePointsMJD_UTC[i] = timeStartMJD_UTC + static_cast<scalar>(i);
    }

    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[0], timeStartMJD_UTC);
    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[timePointsMJD_UTC.size() - 1], timeEndMJD_UTC);

    const Ballistics::FrameModule::PolarMotionContainer polarMotionContainer(timePointsMJD_UTC, xpValues, ypValues);

    for (Ballistics::indexType i = 0; i < xpValues.size() - 1; ++i) {

        const scalar referenceInterpolatedXp = xpValues[i];
        const scalar referenceInterpolatedYp = ypValues[i];

        const scalar midPointMJD_UTC = timePointsMJD_UTC[i];
        const Time<Ballistics::TimeModule::TimeScale::UTC_SCALE> utc = Time<Ballistics::TimeModule::TimeScale::UTC_SCALE>::buildFromMJD(
                midPointMJD_UTC);

        ASSERT_DOUBLE_EQ(utc.mjd(), midPointMJD_UTC);

        const Ballistics::FrameModule::PolarMotion polarMotion = polarMotionContainer.getPolarMotion(utc);

        ASSERT_DOUBLE_EQ(polarMotion.xp_, referenceInterpolatedXp);
        ASSERT_DOUBLE_EQ(polarMotion.yp_, referenceInterpolatedYp);
    }
}


TEST(INTERPOLATOR, RIGHT_POINTS) {

    const Ballistics::Containers::vector<scalar> xpValues = {
            0.095386,
            0.093289,
            0.090829,
            0.088414,
            0.086360,
            0.084450,
            0.081750,
            0.079016,
            0.076411,
            0.074159,
            0.072162};

    const Ballistics::Containers::vector<scalar> ypValues = {
            0.268935,
            0.269801,
            0.270475,
            0.270763,
            0.271107,
            0.272019,
            0.272569,
            0.272577,
            0.272536,
            0.272895,
            0.273364
    };

    const scalar timeStartMJD_UTC = 58480;
    const scalar timeEndMJD_UTC = 58490;

    Ballistics::Containers::vector<scalar> timePointsMJD_UTC(xpValues.size());

    for (Ballistics::indexType i = 0; i < xpValues.size(); ++i) {
        timePointsMJD_UTC[i] = timeStartMJD_UTC + static_cast<scalar>(i);
    }

    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[0], timeStartMJD_UTC);
    ASSERT_DOUBLE_EQ(timePointsMJD_UTC[timePointsMJD_UTC.size() - 1], timeEndMJD_UTC);

    const Ballistics::FrameModule::PolarMotionContainer polarMotionContainer(timePointsMJD_UTC, xpValues, ypValues);

    for (Ballistics::indexType i = 0; i < xpValues.size() - 2; ++i) {

        const scalar referenceInterpolatedXp = xpValues[i + 1];
        const scalar referenceInterpolatedYp = ypValues[i + 1];

        const scalar midPointMJD_UTC = timePointsMJD_UTC[i + 1];
        const Time<Ballistics::TimeModule::TimeScale::UTC_SCALE> utc = Time<Ballistics::TimeModule::TimeScale::UTC_SCALE>::buildFromMJD(
                midPointMJD_UTC);

        ASSERT_DOUBLE_EQ(utc.mjd(), midPointMJD_UTC);

        const Ballistics::FrameModule::PolarMotion polarMotion = polarMotionContainer.getPolarMotion(utc);

        ASSERT_DOUBLE_EQ(polarMotion.xp_, referenceInterpolatedXp);
        ASSERT_DOUBLE_EQ(polarMotion.yp_, referenceInterpolatedYp);
    }
}