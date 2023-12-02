//
// Created by ivankhripunov on 02.12.23.
//

#include <gtest/gtest.h>
#include "../../src/utility/types/BasicTypes.h"
#include "../../src/utility/parser/BulletinParser.h"

TEST(PARSER, SET1) {

    const auto begin = static_cast<Ballistics::scalar>(37665);
    const auto end = static_cast<Ballistics::scalar>(37680);
    const std::string path = "/home/ivankhripunov/CLionProjects/ballistics2023/data/eopc04_IAU2000.62-now";
    const Ballistics::Utility::MJD_DUT mjd_dutData = Ballistics::Utility::getMJD_DUT(path, begin, end);

    for (Ballistics::indexType i = 0; i < mjd_dutData.mjdVector.size(); ++i) {
        ASSERT_DOUBLE_EQ(mjd_dutData.mjdVector[i], begin + static_cast<Ballistics::scalar>(i));
    }

    ASSERT_DOUBLE_EQ(mjd_dutData.dutVector[0], static_cast<Ballistics::scalar>(0.0326338));
    ASSERT_DOUBLE_EQ(mjd_dutData.dutVector[mjd_dutData.dutVector.size() - 1],
                     static_cast<Ballistics::scalar>(0.0261751));
}