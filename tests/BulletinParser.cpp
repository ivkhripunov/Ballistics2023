//
// Created by ivankhripunov on 02.12.23.
//

#include <gtest/gtest.h>
#include "../../src/utility/types/BasicTypes.h"
#include "../../src/utility/parser/BulletinParser.h"

//Данные в скачанной таблице и в csv разные!

TEST(PARSER, SET1) {


    const Ballistics::Containers::vector<Ballistics::scalar> referenceDutValues = {
            -0.0320194,
            -0.0330747,
            -0.0341613,
            -0.0351992,
            -0.0361567,
            -0.0370275,
            -0.0377567
    };

    const auto begin = static_cast<Ballistics::scalar>(58480);
    const auto end = static_cast<Ballistics::scalar>(58486);
    const Ballistics::Containers::string path = "/home/ivankhripunov/CLionProjects/ballistics2023/data/eopc04_IAU2000.62-now";
    const Ballistics::Utility::MJD_DUT mjd_dutData = Ballistics::Utility::getMJD_DUT(path, begin, end);

    for (Ballistics::indexType i = 0; i < mjd_dutData.mjdVector.size(); ++i) {
        ASSERT_DOUBLE_EQ(mjd_dutData.mjdVector[i], begin + static_cast<Ballistics::scalar>(i));

        ASSERT_DOUBLE_EQ(mjd_dutData.dutVector[i], referenceDutValues[i]);
    }
}