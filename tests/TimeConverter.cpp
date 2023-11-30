//
// Created by ivankhripunov on 30.11.23.
//

#include <gtest/gtest.h>
#include <iostream>
#include "types/BasicTypes.h"
#include "time/TimeConverter.h"
#include "../data/time_result.hpp"

using scalar = Ballistics::scalar;

template<typename RealType, typename DutContainer>
using TimeConverter = Ballistics::TimeModule::TimeConverter<RealType, DutContainer>;

template<typename RealType, Ballistics::TimeModule::TimeScale Scale>
using Time = Ballistics::TimeModule::Time<RealType, Scale>;

using TimeScale = Ballistics::TimeModule::TimeScale;

TEST(CONVERT, TAI_UTC) {

    //на этом этапе еще не был реализован дут контейнер, поэтому это просто 0
    TimeConverter<scalar, scalar> converter(0);

    for (const auto& timeSet : Ballistics::timeResult) {
        Time<scalar, TimeScale::TAI_SCALE> timeTAI(timeSet[5], timeSet[6]);

        const auto timeUTC = converter.convert<TimeScale::UTC_SCALE>(timeTAI);

        Time<scalar, TimeScale::UTC_SCALE> referenceUTC(timeSet[3], timeSet[4]);

        ASSERT_EQ(timeUTC.scale(), referenceUTC.scale());
        ASSERT_DOUBLE_EQ(timeUTC.jdDayFrac(), referenceUTC.jdDayFrac());
        ASSERT_DOUBLE_EQ(timeUTC.jdDayInt(), referenceUTC.jdDayInt());
    }

}

TEST(CONVERT, TAI_UT1) {

    //на этом этапе еще не был реализован дут контейнер, поэтому это просто 0
    TimeConverter<scalar, scalar> converter(0);

    for (const auto& timeSet : Ballistics::timeResult) {
        Time<scalar, TimeScale::TAI_SCALE> timeTAI(timeSet[5], timeSet[6]);

        const auto timeUTC = converter.convert<TimeScale::UTC_SCALE>(timeTAI);

        Time<scalar, TimeScale::UTC_SCALE> referenceUTC(timeSet[3], timeSet[4]);

        ASSERT_EQ(timeUTC.scale(), referenceUTC.scale());
        ASSERT_DOUBLE_EQ(timeUTC.jdDayFrac(), referenceUTC.jdDayFrac());
        ASSERT_DOUBLE_EQ(timeUTC.jdDayInt(), referenceUTC.jdDayInt());
    }

}
