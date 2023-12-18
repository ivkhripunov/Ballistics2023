//
// Created by ivankhripunov on 18.12.23.
//

#include <gtest/gtest.h>
#include "ephemeris/EphemerisCalculator.h"
#include "forces/Gravity/EarthGravity.h"
#include "forces/Gravity/CelestialGravity.h"

TEST(GRAVITY, EARTH_COMPARISON) {

    const double tolerance = 1e-6;

    Ballistics::Force::EarthGravityForce earthGravity4x4("/home/ivankhripunov/CLionProjects/ballistics2023/data/",
                                                         "egm96", 4, 4);

    const Ballistics::Containers::string ephemerisPath = "/home/ivankhripunov/CLionProjects/ballistics2023/data/de405.bin";
    const Ballistics::Ephemeris::EphemerisCalculator ephemerisCalculator(ephemerisPath);

    const double R = 42164e3;
    const double mass = 100;

    const int target = 3;
    const double gravParameter = ephemerisCalculator.calcGravParameter(target);
    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> tt(0, 0);
    const Ballistics::Force::CelestialGravity::SatParams satParams{};
    const Ballistics::Force::CelestialGravity celestialGravity(gravParameter);

    for (int m = 0; m < 100; m++) {
        for (int n = 0; n < 100; n++) {

            double Tet = M_1_PI / 100 * m;
            double Phi = 2 * M_1_PI / 100 * n;

            double x = R * std::sin(Tet) * std::cos(Phi);
            double y = R * std::sin(Tet) * std::sin(Phi);
            double z = R * std::cos(Tet);

            const Ballistics::Vector3d position = {x, y, z};
            const Ballistics::Vector3d velocity = Ballistics::Vector3d::Zero();

            const Ballistics::Vector3d egmAcceleration = earthGravity4x4.calcAcceleration({x, y, z}, mass);
            const Ballistics::Vector3d celestialAcceleration = celestialGravity.calcAcceleration(tt, position, velocity,
                                                                                                 mass, satParams);

            ASSERT_NEAR((egmAcceleration - celestialAcceleration).norm(), 0, tolerance);
        }
    }
}
