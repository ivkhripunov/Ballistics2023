//
// Created by ivankhripunov on 20.12.23.
//

#include <gtest/gtest.h>
#include "solar/ShadowModel.h"

TEST(PENUMBRA, SET1) {

    const double tolerance = 1e-3;

    const Ballistics::Vector3d objectPosition = {0, 0, 0};
    const Ballistics::Vector3d bodyPosition = {10, 0, 0};
    const Ballistics::Vector3d sunPosition = {20, 0, 0};

    const Ballistics::Solar::PenumbraModel shadowModel(2, 0.5);

    const double shadowFactor = shadowModel.calcShadowFunction(objectPosition, sunPosition, bodyPosition);
    const double referenceShadowFactor = 0.75;

    ASSERT_NEAR(shadowFactor, referenceShadowFactor, tolerance);
}

TEST(PENUMBRA, SET2) {

    const double tolerance = 1e-15;

    const Ballistics::Vector3d objectPosition = {0, 0, 0};
    const Ballistics::Vector3d bodyPosition = {1, 0, 0};
    const Ballistics::Vector3d sunPosition = {2, 0, 0};

    const Ballistics::Solar::PenumbraModel shadowModel(1, 0.5);

    const double shadowFactor = shadowModel.calcShadowFunction(objectPosition, sunPosition, bodyPosition);
    const double referenceShadowFactor = 0;

    ASSERT_NEAR(shadowFactor, referenceShadowFactor, tolerance);
}

TEST(PENUMBRA, SET3) {

    const double tolerance = 1e-15;

    const Ballistics::Vector3d objectPosition = {1, 0, 0};
    const Ballistics::Vector3d bodyPosition = {0, 0, 0};
    const Ballistics::Vector3d sunPosition = {2, 0, 0};

    const Ballistics::Solar::PenumbraModel shadowModel(1, 0.5);

    const double shadowFactor = shadowModel.calcShadowFunction(objectPosition, sunPosition, bodyPosition);
    const double referenceShadowFactor = 1;

    ASSERT_NEAR(shadowFactor, referenceShadowFactor, tolerance);
}