//
// Created by ivankhripunov on 20.12.23.
//

#include <gtest/gtest.h>
#include "solar/ShadowModel.h"

TEST(PENUMBRA, SET1) {

    const double tolerance = 1e-3;

    Ballistics::Vector3d object = {0, 0, 0};
    Ballistics::Vector3d body = {10, 0, 0};
    Ballistics::Vector3d sun = {20, 0, 0};

    Ballistics::Solar::PenumbraModel penumbraModel(2, 0.5);

    const double shadowFactor = penumbraModel.calcShadowFunction(object, sun, body);
    const double referenceShadowFactor = 0.75;

    ASSERT_NEAR(shadowFactor, referenceShadowFactor, tolerance);
}

TEST(PENUMBRA, SET2) {

    const double tolerance = 1e-15;

    Ballistics::Vector3d object = {0, 0, 0};
    Ballistics::Vector3d body = {1, 0, 0};
    Ballistics::Vector3d sun = {2, 0, 0};

    Ballistics::Solar::PenumbraModel penumbraModel(1, 0.5);

    const double shadowFactor = penumbraModel.calcShadowFunction(object, sun, body);
    const double referenceShadowFactor = 0;

    ASSERT_NEAR(shadowFactor, referenceShadowFactor, tolerance);
}

TEST(PENUMBRA, SET3) {

    const double tolerance = 1e-15;

    Ballistics::Vector3d object = {1, 0, 0};
    Ballistics::Vector3d body = {0, 0, 0};
    Ballistics::Vector3d sun = {2, 0, 0};

    Ballistics::Solar::PenumbraModel penumbraModel(1, 0.5);

    const double shadowFactor = penumbraModel.calcShadowFunction(object, sun, body);
    const double referenceShadowFactor = 1;

    ASSERT_NEAR(shadowFactor, referenceShadowFactor, tolerance);
}