//
// Created by ivankhripunov on 19.12.23.
//

#include <gtest/gtest.h>
#include "GOST4401_81.h"
#include "forces/AtmosphereDrag/AtmosphereDrag.h"

TEST(DRAG, SET1) {

    Ballistics::Containers::vector<Ballistics::Utility::Interpolator<Ballistics::scalar, Ballistics::scalar>::XY> xy(
            Ballistics::Atmosphere::GOST4401_81Raw::N);

    for (Ballistics::indexType i = 0; i < Ballistics::Atmosphere::GOST4401_81Raw::N; ++i) {
        xy[i] = {Ballistics::Atmosphere::GOST4401_81Raw::height[i],
                 Ballistics::Atmosphere::GOST4401_81Raw::density[i]};
    }

    const Ballistics::Utility::Interpolator<Ballistics::scalar, Ballistics::scalar> interpolator(xy);

    const Ballistics::Atmosphere::DensityCalculator densityCalculator(interpolator);

    const Ballistics::Force::AtmosphereDrag atmosphereDrag(densityCalculator);

    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> timeTT(2458119, 0.5);
    const Ballistics::Vector3d position = {6800e3, 0, 0};
    const Ballistics::Vector3d velocity = {0, std::sqrt(398600e9 / 6800e3), 0};
    const double mass = 1;
    decltype(atmosphereDrag)::SatParams satParams = {1, 1};
    const double inputParams = 0;

    const Ballistics::Vector3d drag = atmosphereDrag.calcAcceleration(timeTT, position, velocity, mass, satParams, inputParams);

    std::cout << drag;
}
