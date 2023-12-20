//
// Created by ivankhripunov on 17.12.23.
//

#include <gtest/gtest.h>
#include "ephemeris/EphemerisCalculator.h"
#include "time/DutCorrection.h"
#include "time/TimeConverter.h"
#include "utility/parser/BulletinParser.h"
#include "ephemeris/ephemeris_result.h"

TEST(EPHEMERIS, POSVEL) {
    const auto tolerance = static_cast<Ballistics::scalar>(1);

    const std::string currentFile = __FILE__;
    const std::string ephemerisPath = currentFile.substr(0, currentFile.size() - 44) + "data/ephemeris/de405.bin";

    const Ballistics::Ephemeris::EphemerisCalculator ephemerisCalculator(ephemerisPath);

    const auto begin = static_cast<Ballistics::scalar>(0);
    const auto end = static_cast<Ballistics::scalar>(100000);

    const Ballistics::Containers::string mjdDutPath = "../../../data/frame/earth_rotation.csv";
    const Ballistics::Utility::MJD_DUT mjd_dut = Ballistics::Utility::CSVgetMJD_DUT(mjdDutPath, begin, end);

    const Ballistics::TimeModule::DutContainer dutContainer(mjd_dut.mjdVector, mjd_dut.dutVector);

    const Ballistics::TimeModule::TimeConverter<Ballistics::TimeModule::DutContainer> timeConverter(dutContainer);

    for (const auto &set: ephemeris_result) {

        const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> tt(set[1], set[2]);
        const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TDB_SCALE> tdb = timeConverter.convert<Ballistics::TimeModule::TimeScale::TDB_SCALE>(
                tt);

        const auto targetBody = static_cast<int>(set[0]);
        const auto centerBody = 3;
        const Ballistics::Containers::vector<double> result = ephemerisCalculator.calculateBodyRV(
                targetBody, centerBody, tdb);

        ASSERT_EQ(result.size(), 6);

        const Ballistics::Containers::vector<double> reference = {set[3], set[4], set[5], set[6],
                                                                              set[7], set[8]};

        for (Ballistics::indexType i = 0; i < reference.size(); ++i) {
            ASSERT_NEAR(result[i], reference[i], tolerance);
        }
    }
}

TEST(EPHEMERIS, POS) {
    const auto tolerance = static_cast<Ballistics::scalar>(1);

    const std::string currentFile = __FILE__;
    const std::string ephemerisPath = currentFile.substr(0, currentFile.size() - 44) + "data/ephemeris/de405.bin";

    const Ballistics::Ephemeris::EphemerisCalculator ephemerisCalculator(ephemerisPath);

    const auto begin = static_cast<Ballistics::scalar>(0);
    const auto end = static_cast<Ballistics::scalar>(100000);

    const Ballistics::Containers::string mjdDutPath = "../../../data/frame/earth_rotation.csv";
    const Ballistics::Utility::MJD_DUT mjd_dut = Ballistics::Utility::CSVgetMJD_DUT(mjdDutPath, begin, end);

    const Ballistics::TimeModule::DutContainer dutContainer(mjd_dut.mjdVector, mjd_dut.dutVector);

    const Ballistics::TimeModule::TimeConverter<Ballistics::TimeModule::DutContainer> timeConverter(dutContainer);

    for (const auto &set: ephemeris_result) {

        const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> tt(set[1], set[2]);
        const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TDB_SCALE> tdb = timeConverter.convert<Ballistics::TimeModule::TimeScale::TDB_SCALE>(
                tt);

        const auto targetBody = static_cast<int>(set[0]);
        const auto centerBody = 3;
        const Ballistics::Containers::vector<double> result = ephemerisCalculator.calculateBodyR(
                targetBody, centerBody, tdb);

        ASSERT_EQ(result.size(), 3);

        const Ballistics::Containers::vector<double> reference = {set[3], set[4], set[5]};

        for (Ballistics::indexType i = 0; i < reference.size(); ++i) {
            ASSERT_NEAR(result[i], reference[i], tolerance);
        }
    }
}
