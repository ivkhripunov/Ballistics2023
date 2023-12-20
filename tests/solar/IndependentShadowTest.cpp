//
// Created by ivankhripunov on 20.12.23.
//

#include <gtest/gtest.h>
#include "solar/ShadowModel.h"
#include "solar/IndependentShadow.h"

TEST(INDEPENDENT, SET1) {
   Ballistics::Vector3d object = {-20000, 0, 0};
   Ballistics::Vector3d earth = {0, 0, 0};
   Ballistics::Vector3d moon = {0, 0, 0};
   Ballistics::Vector3d sun = {80000, 0, 0};

    const double earth_rad = 2000;
    const double moon_rad = 500;
    const double sun_rad = 30000;
    Ballistics::Solar::PenumbraModel earthShadow_(sun_rad, earth_rad);
    Ballistics::Solar::PenumbraModel moonShadow_(sun_rad, moon_rad);

    double F_es = earthShadow_.calcShadowFunction(object, sun, earth);
    double F_ms = moonShadow_.calcShadowFunction(object, sun, moon);

    Ballistics::Solar::IndependentShadow independent(earthShadow_, moonShadow_);

    ASSERT_NEAR(std::min(F_ms, F_es), F_es, 1e-16);

}
