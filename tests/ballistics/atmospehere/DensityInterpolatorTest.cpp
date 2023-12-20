//
// Created by ivankhripunov on 19.12.23.
//

#include <gtest/gtest.h>
#include "atmosphere/DensityCalculator.h"
#include "atmosphere/GOST4401_81.h"

TEST(DENSITY, GOST) {

    const auto tolerance = static_cast<Ballistics::scalar>(1e-12);

    Ballistics::Containers::vector<Ballistics::Utility::Interpolator<Ballistics::scalar, Ballistics::scalar>::XY> xy(
            Ballistics::Atmosphere::GOST4401_81Raw::N);

    for (Ballistics::indexType i = 0; i < Ballistics::Atmosphere::GOST4401_81Raw::N; ++i) {
        xy[i] = {Ballistics::Atmosphere::GOST4401_81Raw::height[i],
                 Ballistics::Atmosphere::GOST4401_81Raw::density[i]};
    }

    const Ballistics::Utility::Interpolator<Ballistics::scalar, Ballistics::scalar> interpolator(xy);

    const Ballistics::Atmosphere::DensityCalculator densityCalculator(interpolator);

    for (Ballistics::indexType i = 0; i < Ballistics::Atmosphere::GOST4401_81Raw::N - 1; ++i) {

        const Ballistics::scalar point = (Ballistics::Atmosphere::GOST4401_81Raw::height[i] +
                                          Ballistics::Atmosphere::GOST4401_81Raw::height[i + 1]) / 2;

        const Ballistics::scalar density = densityCalculator.calcDensity(point);

        const Ballistics::scalar referenceDensity = (Ballistics::Atmosphere::GOST4401_81Raw::density[i] +
                                                     Ballistics::Atmosphere::GOST4401_81Raw::density[i + 1]) / 2;

        ASSERT_NEAR(density, referenceDensity, tolerance);
    }
}

