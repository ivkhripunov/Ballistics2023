//
// Created by ivankhripunov on 19.12.23.
//

#include <gtest/gtest.h>

#include "solar/IndependentShadow.h"
#include "solar/ShadowModel.h"
#include "solar/SolarPressure.h"
#include "solar/SolarIrradiance.h"

TEST(SOLAR, PRESSURE) {

    const double sunRadius = 696340e3;
    const double earthRadius = 6371e3;
    const double moonRadius = 1737.4e3;

    const Ballistics::Solar::ConstantTSI tsiModel;

    const Ballistics::Solar::PenumbraModel earthShadow(sunRadius, earthRadius);
    const Ballistics::Solar::PenumbraModel moonShadow(sunRadius, moonRadius);

    const Ballistics::Solar::IndependentShadow independentShadow(earthShadow, moonShadow);

    const Ballistics::Solar::SolarPressureCalculator solarPressureCalculator(independentShadow, tsiModel);

}
