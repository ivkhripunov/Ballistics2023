//
// Created by ivankhripunov on 17.12.23.
//

#include <gtest/gtest.h>
#include "ephemeris/EphemerisCalculator.h"

TEST(EPHEMERIS, GRAVPARAM) {

    const double tolerance = 1e-3;

    const std::string currentFile = __FILE__;
    const std::string ephemerisPath = currentFile.substr(0, currentFile.size() - 48) + "data/ephemeris/de405.bin";

    const Ballistics::Ephemeris::EphemerisCalculator ephemerisCalculator(ephemerisPath);

    const double muMoon = ephemerisCalculator.calcGravParameter(10);
    const double muEarth = ephemerisCalculator.calcGravParameter(3);
    const double muJupiter = ephemerisCalculator.calcGravParameter(5);
    const double muSun = ephemerisCalculator.calcGravParameter(11);

    const double referenceMuMoon = 	4.9028695e12;
    const double referenceMuEarth = 3.986004418e14;
    const double referenceJupiter = 1.26713e17;
    const double referenceMuSun = 	1.32712440018e20;

    ASSERT_NEAR(std::abs(muMoon - referenceMuMoon) / referenceMuMoon, 0, tolerance);
    ASSERT_NEAR(std::abs(muEarth - referenceMuEarth) / referenceMuEarth, 0, tolerance);
    ASSERT_NEAR(std::abs(muJupiter - referenceJupiter) / muJupiter, 0, tolerance);
    ASSERT_NEAR(std::abs(muSun - referenceMuSun) / referenceMuSun, 0, tolerance);
}