//
// Created by ivankhripunov on 30.11.23.
//

#include <gtest/gtest.h>
#include <iostream>
#include "time/TimeConverter.h"
#include "time/DutCorrection.h"
#include "time/time_result.hpp"
#include "utility/parser/BulletinParser.h"

//TODO: fix

//Здесь дуты заданы по csv табличке!!

using scalar = Ballistics::scalar;

template<typename DutContainer>
using TimeConverter = Ballistics::TimeModule::TimeConverter<DutContainer>;
using DutContainer = Ballistics::TimeModule::DutContainer;

template<Ballistics::TimeModule::TimeScale Scale>
using Time = Ballistics::TimeModule::Time<Scale>;

using TimeScale = Ballistics::TimeModule::TimeScale;

TEST(CONVERT, UTC_UT1_TAI) {

    const Ballistics::Containers::vector<scalar> dutValues = {
            -0.0320059,
            -0.0330198,
            -0.0340678,
            -0.0351611,
            -0.0361345,
            -0.0370076,
            -0.0377478
    };

    const scalar timeStartMJD_UTC = 58480;
    const scalar timeEndMJD_UTC = 58486;

    Ballistics::Containers::vector<scalar> timePointsMJD_UTC(dutValues.size());

    for (Ballistics::indexType i = 0; i < dutValues.size(); ++i) {
        timePointsMJD_UTC[i] = timeStartMJD_UTC + static_cast<scalar>(i);
    }

    const DutContainer dutContainer(timePointsMJD_UTC, dutValues);

    const TimeConverter<DutContainer> timeConverter(dutContainer);

    const Time<TimeScale::UTC_SCALE> utc(Ballistics::timeResult[0][3], Ballistics::timeResult[0][4]);

    const Time<TimeScale::UT1_SCALE> ut1 = timeConverter.convertUTC_UT1(utc);
    const Time<TimeScale::TAI_SCALE> tai = timeConverter.convertUTC_TAI(utc);

    const Time<TimeScale::UT1_SCALE> ut1_reference(Ballistics::timeResult[0][1], Ballistics::timeResult[0][2]);
    const Time<TimeScale::TAI_SCALE> tai_reference(Ballistics::timeResult[0][5], Ballistics::timeResult[0][6]);

    ASSERT_DOUBLE_EQ(ut1.jdDayInt(), ut1_reference.jdDayInt());
    ASSERT_DOUBLE_EQ(ut1.jdDayFrac(), ut1_reference.jdDayFrac());

    ASSERT_DOUBLE_EQ(tai.jdDayInt(), tai_reference.jdDayInt());
    ASSERT_DOUBLE_EQ(tai.jdDayFrac(), tai_reference.jdDayFrac());

}

TEST(CONVERT, MPI_UT1_UTC) {

    const Ballistics::Containers::vector<scalar> dutValues = {
            -0.0320059,
            -0.0330198,
            -0.0340678,
            -0.0351611,
            -0.0361345,
            -0.0370076,
            -0.0377478
    };

    const scalar timeStartMJD_UTC = 58480;
    const scalar timeEndMJD_UTC = 58486;

    Ballistics::Containers::vector<scalar> timePointsMJD_UTC(dutValues.size());

    for (Ballistics::indexType i = 0; i < dutValues.size(); ++i) {
        timePointsMJD_UTC[i] = timeStartMJD_UTC + static_cast<scalar>(i);
    }

    const DutContainer dutContainer(timePointsMJD_UTC, dutValues);

    const TimeConverter<DutContainer> timeConverter(dutContainer);

    const Time<TimeScale::UTC_SCALE> utc_reference(Ballistics::timeResult[0][3], Ballistics::timeResult[0][4]);

    const Time<TimeScale::UT1_SCALE> ut1(Ballistics::timeResult[0][1], Ballistics::timeResult[0][2]);
    const Time<TimeScale::UTC_SCALE> utc = timeConverter.convertUT1_UTC(ut1);

    ASSERT_DOUBLE_EQ(utc.jdDayInt(), utc_reference.jdDayInt());
    ASSERT_DOUBLE_EQ(utc.jdDayFrac(), utc_reference.jdDayFrac());
}

TEST(CONVERT, TT_TDB) {

    const Ballistics::Containers::vector<scalar> dutValues = {
            -0.0320059,
            -0.0330198,
            -0.0340678,
            -0.0351611,
            -0.0361345,
            -0.0370076,
            -0.0377478
    };

    const scalar timeStartMJD_UTC = 58480;
    const scalar timeEndMJD_UTC = 58486;

    Ballistics::Containers::vector<scalar> timePointsMJD_UTC(dutValues.size());

    for (Ballistics::indexType i = 0; i < dutValues.size(); ++i) {
        timePointsMJD_UTC[i] = timeStartMJD_UTC + static_cast<scalar>(i);
    }

    const DutContainer dutContainer(timePointsMJD_UTC, dutValues);

    const TimeConverter<DutContainer> timeConverter(dutContainer);

    const Time<TimeScale::TDB_SCALE> tdb_reference(Ballistics::timeResult[0][13], Ballistics::timeResult[0][14]);

    const Time<TimeScale::TT_SCALE> tt(Ballistics::timeResult[0][7], Ballistics::timeResult[0][8]);
    const Time<TimeScale::TDB_SCALE> tdb = timeConverter.convertTT_TDB(tt);

    ASSERT_DOUBLE_EQ(tdb.jdDayInt(), tdb_reference.jdDayInt());
    ASSERT_DOUBLE_EQ(tdb.jdDayFrac(), tdb_reference.jdDayFrac());
}

TEST(CONVERT, TDB_TT_MPI) {

    const Ballistics::Containers::vector<scalar> dutValues = {
            -0.0320059,
            -0.0330198,
            -0.0340678,
            -0.0351611,
            -0.0361345,
            -0.0370076,
            -0.0377478
    };

    const scalar timeStartMJD_UTC = 58480;
    const scalar timeEndMJD_UTC = 58486;

    Ballistics::Containers::vector<scalar> timePointsMJD_UTC(dutValues.size());

    for (Ballistics::indexType i = 0; i < dutValues.size(); ++i) {
        timePointsMJD_UTC[i] = timeStartMJD_UTC + static_cast<scalar>(i);
    }

    const DutContainer dutContainer(timePointsMJD_UTC, dutValues);

    const TimeConverter<DutContainer> timeConverter(dutContainer);

    const Time<TimeScale::TDB_SCALE> tdb(Ballistics::timeResult[0][13], Ballistics::timeResult[0][14]);

    const Time<TimeScale::TT_SCALE> tt_reference(Ballistics::timeResult[0][7], Ballistics::timeResult[0][8]);
    const Time<TimeScale::TT_SCALE> tt = timeConverter.convertTDB_TT(tdb);

    ASSERT_DOUBLE_EQ(tt.jdDayInt(), tt_reference.jdDayInt());
    ASSERT_DOUBLE_EQ(tt.jdDayFrac(), tt_reference.jdDayFrac());
}

TEST(CONVERT, SET1) {
    const auto tolerance = static_cast<scalar>(2e-11);
    const Ballistics::Containers::vector<TimeScale> scales = {TimeScale::UT1_SCALE, TimeScale::UTC_SCALE,
                                                              TimeScale::TAI_SCALE, TimeScale::TT_SCALE,
                                                              TimeScale::TCG_SCALE, TimeScale::TCB_SCALE,
                                                              TimeScale::TDB_SCALE};

    const auto begin = static_cast<Ballistics::scalar>(0);
    const auto end = static_cast<Ballistics::scalar>(100000);

    const Ballistics::Containers::string path = "/home/ivankhripunov/CLionProjects/ballistics2023/data/frame/earth_rotation.csv";
    const Ballistics::Utility::MJD_DUT mjd_dut = Ballistics::Utility::CSVgetMJD_DUT(path, begin, end);

    const DutContainer dutContainer(mjd_dut.mjdVector, mjd_dut.dutVector);

    const TimeConverter<DutContainer> timeConverter(dutContainer);

    for (const auto &timePoint: Ballistics::timeResult) {

        const Time<TimeScale::UT1_SCALE> ut1_reference(timePoint[1], timePoint[2]);
        const Time<TimeScale::UTC_SCALE> utc_reference(timePoint[3], timePoint[4]);
        const Time<TimeScale::TAI_SCALE> tai_reference(timePoint[5], timePoint[6]);
        const Time<TimeScale::TT_SCALE> tt_reference(timePoint[7], timePoint[8]);
        const Time<TimeScale::TCG_SCALE> tcg_reference(timePoint[9], timePoint[10]);
        const Time<TimeScale::TCB_SCALE> tcb_reference(timePoint[11], timePoint[12]);
        const Time<TimeScale::TDB_SCALE> tdb_reference(timePoint[13], timePoint[14]);

        /****************************************to UT1******************************************************/

        const Time<TimeScale::UT1_SCALE> ut1_ut1 = timeConverter.convert<TimeScale::UT1_SCALE>(ut1_reference);
        ASSERT_NEAR(static_cast<double>(ut1_ut1 - ut1_reference), 0, tolerance);

        const Time<TimeScale::UT1_SCALE> utc_ut1 = timeConverter.convert<TimeScale::UT1_SCALE>(utc_reference);
        ASSERT_NEAR(static_cast<double>(utc_ut1 - ut1_reference), 0, tolerance);

        const Time<TimeScale::UT1_SCALE> tai_ut1 = timeConverter.convert<TimeScale::UT1_SCALE>(tai_reference);
        ASSERT_NEAR(static_cast<double>(tai_ut1- ut1_reference), 0, tolerance);

        const Time<TimeScale::UT1_SCALE> tt_ut1 = timeConverter.convert<TimeScale::UT1_SCALE>(tt_reference);
        ASSERT_NEAR(static_cast<double>(tt_ut1 - ut1_reference), 0, tolerance);

        const Time<TimeScale::UT1_SCALE> tcg_ut1 = timeConverter.convert<TimeScale::UT1_SCALE>(tcg_reference);
        ASSERT_NEAR(static_cast<double>(tcg_ut1 - ut1_reference), 0, tolerance);

        const Time<TimeScale::UT1_SCALE> tcb_ut1 = timeConverter.convert<TimeScale::UT1_SCALE>(tcb_reference);
        ASSERT_NEAR(static_cast<double>(tcb_ut1 - ut1_reference), 0, tolerance * 10);
        //Тут функция для dtr с точностью 50 микросекунд
        const Time<TimeScale::UT1_SCALE> tdb_ut1 = timeConverter.convert<TimeScale::UT1_SCALE>(tcb_reference);
        ASSERT_NEAR(static_cast<double>(tdb_ut1 - ut1_reference), 0, tolerance * 10);

        /****************************************to UTC******************************************************/

        const Time<TimeScale::UTC_SCALE> ut1_utc = timeConverter.convert<TimeScale::UTC_SCALE>(ut1_reference);
        ASSERT_NEAR(static_cast<double>(ut1_utc - utc_reference), 0, tolerance);

        const Time<TimeScale::UTC_SCALE> utc_utc = timeConverter.convert<TimeScale::UTC_SCALE>(utc_reference);
        ASSERT_NEAR(static_cast<double>(utc_utc - utc_reference), 0, tolerance);

        const Time<TimeScale::UTC_SCALE> tai_utc = timeConverter.convert<TimeScale::UTC_SCALE>(tai_reference);
        ASSERT_NEAR(static_cast<double>(tai_utc- utc_reference), 0, tolerance);

        const Time<TimeScale::UTC_SCALE> tt_utc = timeConverter.convert<TimeScale::UTC_SCALE>(tt_reference);
        ASSERT_NEAR(static_cast<double>(tt_utc - utc_reference), 0, tolerance);

        const Time<TimeScale::UTC_SCALE> tcg_utc = timeConverter.convert<TimeScale::UTC_SCALE>(tcg_reference);
        ASSERT_NEAR(static_cast<double>(tcg_utc - utc_reference), 0, tolerance);

        const Time<TimeScale::UTC_SCALE> tcb_utc = timeConverter.convert<TimeScale::UTC_SCALE>(tcb_reference);
        ASSERT_NEAR(static_cast<double>(tcb_utc - utc_reference), 0, tolerance * 10);
        //Тут функция для dtr с точностью 50 микросекунд
        const Time<TimeScale::UTC_SCALE> tdb_utc = timeConverter.convert<TimeScale::UTC_SCALE>(tcb_reference);
        ASSERT_NEAR(static_cast<double>(tdb_utc - utc_reference), 0, tolerance * 10);

        /****************************************to TAI******************************************************/

        const Time<TimeScale::TAI_SCALE> ut1_tai = timeConverter.convert<TimeScale::TAI_SCALE>(ut1_reference);
        ASSERT_NEAR(static_cast<double>(ut1_tai - tai_reference), 0, tolerance);

        const Time<TimeScale::TAI_SCALE> utc_tai = timeConverter.convert<TimeScale::TAI_SCALE>(utc_reference);
        ASSERT_NEAR(static_cast<double>(utc_tai - tai_reference), 0, tolerance);

        const Time<TimeScale::TAI_SCALE> tai_tai = timeConverter.convert<TimeScale::TAI_SCALE>(tai_reference);
        ASSERT_NEAR(static_cast<double>(tai_tai - tai_reference), 0, tolerance);

        const Time<TimeScale::TAI_SCALE> tt_tai = timeConverter.convert<TimeScale::TAI_SCALE>(tt_reference);
        ASSERT_NEAR(static_cast<double>(tt_tai - tai_reference), 0, tolerance);

        const Time<TimeScale::TAI_SCALE> tcg_tai = timeConverter.convert<TimeScale::TAI_SCALE>(tcg_reference);
        ASSERT_NEAR(static_cast<double>(tcg_tai - tai_reference), 0, tolerance);

        const Time<TimeScale::TAI_SCALE> tcb_tai = timeConverter.convert<TimeScale::TAI_SCALE>(tcb_reference);
        ASSERT_NEAR(static_cast<double>(tcb_tai - tai_reference), 0, tolerance * 10);
        //Тут функция для dtr с точностью 50 микросекунд
        const Time<TimeScale::TAI_SCALE> tdb_tai = timeConverter.convert<TimeScale::TAI_SCALE>(tcb_reference);
        ASSERT_NEAR(static_cast<double>(tdb_tai - tai_reference), 0, tolerance * 10);

        /****************************************to TT******************************************************/

        const Time<TimeScale::TT_SCALE> ut1_tt = timeConverter.convert<TimeScale::TT_SCALE>(ut1_reference);
        ASSERT_NEAR(static_cast<double>(ut1_tt - tt_reference), 0, tolerance);

        const Time<TimeScale::TT_SCALE> utc_tt = timeConverter.convert<TimeScale::TT_SCALE>(utc_reference);
        ASSERT_NEAR(static_cast<double>(utc_tt - tt_reference), 0, tolerance);

        const Time<TimeScale::TT_SCALE> tai_tt = timeConverter.convert<TimeScale::TT_SCALE>(tai_reference);
        ASSERT_NEAR(static_cast<double>(tai_tt - tt_reference), 0, tolerance);

        const Time<TimeScale::TT_SCALE> tt_tt = timeConverter.convert<TimeScale::TT_SCALE>(tt_reference);
        ASSERT_NEAR(static_cast<double>(tt_tt - tt_reference), 0, tolerance);

        const Time<TimeScale::TT_SCALE> tcg_tt = timeConverter.convert<TimeScale::TT_SCALE>(tcg_reference);
        ASSERT_NEAR(static_cast<double>(tcg_tt - tt_reference), 0, tolerance);

        const Time<TimeScale::TT_SCALE> tcb_tt = timeConverter.convert<TimeScale::TT_SCALE>(tcb_reference);
        ASSERT_NEAR(static_cast<double>(tcb_tt - tt_reference), 0, tolerance * 10);
        //Тут функция для dtr с точностью 50 микросекунд
        const Time<TimeScale::TT_SCALE> tdb_tt = timeConverter.convert<TimeScale::TT_SCALE>(tcb_reference);
        ASSERT_NEAR(static_cast<double>(tdb_tt - tt_reference), 0, tolerance * 10);

        /****************************************to TCG******************************************************/

        const Time<TimeScale::TCG_SCALE> ut1_tcg = timeConverter.convert<TimeScale::TCG_SCALE>(ut1_reference);
        ASSERT_NEAR(ut1_tcg.jdDayInt(), tcg_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(ut1_tcg.jdDayFrac(), tcg_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCG_SCALE> utc_tcg = timeConverter.convert<TimeScale::TCG_SCALE>(utc_reference);
        ASSERT_NEAR(utc_tcg.jdDayInt(), tcg_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(utc_tcg.jdDayFrac(), tcg_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCG_SCALE> tai_tcg = timeConverter.convert<TimeScale::TCG_SCALE>(tai_reference);
        ASSERT_NEAR(tai_tcg.jdDayInt(), tcg_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tai_tcg.jdDayFrac(), tcg_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCG_SCALE> tt_tcg = timeConverter.convert<TimeScale::TCG_SCALE>(tt_reference);
        ASSERT_NEAR(tt_tcg.jdDayInt(), tcg_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tt_tcg.jdDayFrac(), tcg_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCG_SCALE> tcg_tcg = timeConverter.convert<TimeScale::TCG_SCALE>(tcg_reference);
        ASSERT_NEAR(tcg_tcg.jdDayInt(), tcg_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tcg_tcg.jdDayFrac(), tcg_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCG_SCALE> tcb_tcg = timeConverter.convert<TimeScale::TCG_SCALE>(tcb_reference);
        ASSERT_NEAR(tcb_tcg.jdDayInt(), tcg_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tcb_tcg.jdDayFrac(), tcg_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCG_SCALE> tdb_tcg = timeConverter.convert<TimeScale::TCG_SCALE>(tcb_reference);
        ASSERT_NEAR(tdb_tcg.jdDayInt(), tcg_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tdb_tcg.jdDayFrac(), tcg_reference.jdDayFrac(), tolerance);

        /****************************************to TCB******************************************************/

        const Time<TimeScale::TCB_SCALE> ut1_tcb = timeConverter.convert<TimeScale::TCB_SCALE>(ut1_reference);
        ASSERT_NEAR(ut1_tcb.jdDayInt(), tcb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(ut1_tcb.jdDayFrac(), tcb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCB_SCALE> utc_tcb = timeConverter.convert<TimeScale::TCB_SCALE>(utc_reference);
        ASSERT_NEAR(utc_tcb.jdDayInt(), tcb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(utc_tcb.jdDayFrac(), tcb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCB_SCALE> tai_tcb = timeConverter.convert<TimeScale::TCB_SCALE>(tai_reference);
        ASSERT_NEAR(tai_tcb.jdDayInt(), tcb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tai_tcb.jdDayFrac(), tcb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCB_SCALE> tt_tcb = timeConverter.convert<TimeScale::TCB_SCALE>(tt_reference);
        ASSERT_NEAR(tt_tcb.jdDayInt(), tcb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tt_tcb.jdDayFrac(), tcb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCB_SCALE> tcg_tcb = timeConverter.convert<TimeScale::TCB_SCALE>(tcg_reference);
        ASSERT_NEAR(tcg_tcb.jdDayInt(), tcb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tcg_tcb.jdDayFrac(), tcb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCB_SCALE> tcb_tcb = timeConverter.convert<TimeScale::TCB_SCALE>(tcb_reference);
        ASSERT_NEAR(tcb_tcb.jdDayInt(), tcb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tcb_tcb.jdDayFrac(), tcb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TCB_SCALE> tdb_tcb = timeConverter.convert<TimeScale::TCB_SCALE>(tcb_reference);
        ASSERT_NEAR(tdb_tcb.jdDayInt(), tcb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tdb_tcb.jdDayFrac(), tcb_reference.jdDayFrac(), tolerance);

        /****************************************to TDB******************************************************/

        const Time<TimeScale::TDB_SCALE> ut1_tdb = timeConverter.convert<TimeScale::TDB_SCALE>(ut1_reference);
        ASSERT_NEAR(ut1_tdb.jdDayInt(), tdb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(ut1_tdb.jdDayFrac(), tdb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TDB_SCALE> utc_tdb = timeConverter.convert<TimeScale::TDB_SCALE>(utc_reference);
        ASSERT_NEAR(utc_tdb.jdDayInt(), tdb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(utc_tdb.jdDayFrac(), tdb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TDB_SCALE> tai_tdb = timeConverter.convert<TimeScale::TDB_SCALE>(tai_reference);
        ASSERT_NEAR(tai_tdb.jdDayInt(), tdb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tai_tdb.jdDayFrac(), tdb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TDB_SCALE> tt_tdb = timeConverter.convert<TimeScale::TDB_SCALE>(tt_reference);
        ASSERT_NEAR(tt_tdb.jdDayInt(), tdb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tt_tdb.jdDayFrac(), tdb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TDB_SCALE> tcg_tdb = timeConverter.convert<TimeScale::TDB_SCALE>(tcg_reference);
        ASSERT_NEAR(tcg_tdb.jdDayInt(), tdb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tcg_tdb.jdDayFrac(), tdb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TDB_SCALE> tcb_tdb = timeConverter.convert<TimeScale::TDB_SCALE>(tcb_reference);
        ASSERT_NEAR(tcb_tdb.jdDayInt(), tdb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tcb_tdb.jdDayFrac(), tdb_reference.jdDayFrac(), tolerance);

        const Time<TimeScale::TDB_SCALE> tdb_tdb = timeConverter.convert<TimeScale::TDB_SCALE>(tdb_reference);
        ASSERT_NEAR(tdb_tdb.jdDayInt(), tdb_reference.jdDayInt(), tolerance);
        ASSERT_NEAR(tdb_tdb.jdDayFrac(), tdb_reference.jdDayFrac(), tolerance);
    }
}