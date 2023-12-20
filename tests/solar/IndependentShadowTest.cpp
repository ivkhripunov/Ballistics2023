//
// Created by ivankhripunov on 20.12.23.
//

#include <gtest/gtest.h>
#include "solar/ShadowModel.h"
#include "solar/IndependentShadow.h"

TEST(INDEPENDENT, SET1) {

    const double tolerance = 1e-16;
    const Ballistics::Vector3d objectPosition = {-20000, 0, 0};
    const Ballistics::Vector3d earthPosition = {0, 0, 0};
    const Ballistics::Vector3d moonPosition = {0, 0, 0};
    const Ballistics::Vector3d sunPosition = {80000, 0, 0};

    const double earthRadius = 2000;
    const double moonRadius = 500;
    const double sunRadius = 30000;
    const Ballistics::Solar::PenumbraModel earthShadow(sunRadius, earthRadius);
    const Ballistics::Solar::PenumbraModel moonShadow(sunRadius, moonRadius);

    const double shadowFactorEarthSun = earthShadow.calcShadowFunction(objectPosition, sunPosition, earthPosition);
    const double shadowFactorMoonSun = moonShadow.calcShadowFunction(objectPosition, sunPosition, moonPosition);

    const Ballistics::Solar::IndependentShadow independentShadow(earthShadow, moonShadow);

    const Ballistics::Vector3d zeroFlux = Ballistics::Vector3d::UnitX();
    const double resultShadowFactor = independentShadow.calcFlux(objectPosition, sunPosition, moonPosition, zeroFlux).x();

    ASSERT_NEAR(resultShadowFactor, shadowFactorEarthSun, tolerance);

}
