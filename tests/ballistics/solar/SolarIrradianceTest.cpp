//
// Created by ivankhripunov on 20.12.23.
//

#include <gtest/gtest.h>
#include "time/Time.h"
#include "solar/SolarIrradiance.h"

TEST(IRRADIANCE, SET1) {

    const Ballistics::Solar::ConstantTSI constantTsi;

    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> timeTT(0, 0);
    const double resultTSI = constantTsi.calcTSI(timeTT);
    const double referenceTSI= 1366;

    ASSERT_DOUBLE_EQ(resultTSI, referenceTSI);

}
