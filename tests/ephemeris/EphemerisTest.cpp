//
// Created by ivankhripunov on 17.12.23.
//

#include <gtest/gtest.h>
#include "ephemeris/EphemerisCalculator.h"
#include "time/DutCorrection.h"
#include "time/TimeConverter.h"
#include "utility/parser/BulletinParser.h"
#include "../data/ephemeris_result.h"

TEST(EPHEMERIS, POSVEL) {
    const Ballistics::scalar tolerance = 1;

    const Ballistics::Containers::string ephemerisPath = "/home/ivankhripunov/CLionProjects/ballistics2023/data/de405.bin";
    const Ballistics::Ephemeris::EphemerisCalculator ephemerisCalculator(ephemerisPath);


    const auto begin = static_cast<Ballistics::scalar>(0);
    const auto end = static_cast<Ballistics::scalar>(100000);

    const Ballistics::Containers::string mjdDutPath = "/home/ivankhripunov/CLionProjects/ballistics2023/data/earth_rotation.csv";
    const Ballistics::Utility::MJD_DUT mjd_dut = Ballistics::Utility::CSVgetMJD_DUT(mjdDutPath, begin, end);

    const Ballistics::TimeModule::DutContainer dutContainer(mjd_dut.mjdVector, mjd_dut.dutVector);

    const Ballistics::TimeModule::TimeConverter<Ballistics::TimeModule::DutContainer> timeConverter(dutContainer);

    for (const auto &set: ephemeris_result) {

        if (set[0] == 10) {

            const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> tt(set[1], set[2]);
            const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TDB_SCALE> tdb = timeConverter.convert<Ballistics::TimeModule::TimeScale::TDB_SCALE>(
                    tt);

            const Ballistics::Containers::vector<Ballistics::scalar> result = ephemerisCalculator.calculateBody<Ballistics::Ephemeris::EphemerisCalculator::CalcType::POSVEL>(
                    static_cast<int>(set[0]), 3, tdb);

            const Ballistics::Containers::vector<Ballistics::scalar> reference = {set[3], set[4], set[5],set[6], set[7], set[8]};

            for (Ballistics::indexType i = 3; i < reference.size(); ++i) {

                ASSERT_NEAR(result[i], reference[i], tolerance);
            }
        }
    }

}
