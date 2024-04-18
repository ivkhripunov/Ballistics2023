//
// Created by ivankhripunov on 18.04.24.
//

#ifndef BALLISTICS2023_DIFFCORRECTION_H
#define BALLISTICS2023_DIFFCORRECTION_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"
#include "utility/numericalMethods/RungeKutta.h"

namespace Ballistics::NumericalMethods {

    template<typename State>
    [[nodiscard]] State
    calcDeltaState(const State &initialState, const scalar delta, const indexType i) {

        State result = initialState;

        if (i < 3) {
            result.position(i) += delta;
        } else {
            result.velocity(i - 3) += delta;
        }

        return result;
    }

    template<indexType N, typename RHS, typename SatParam>
    [[nodiscard]] Vector<scalar, 6>
    diffCorrectionStep(const Eigen::Matrix<scalar, 6, 6> &W, const typename RHS::State &initialState,
                   const std::array<Vector<scalar, 6>, N> &y, const std::array<scalar, 6> &delta,
                   const std::array<TimeModule::Time<TimeModule::TimeScale::TT_SCALE>, N> &time,
                   const RHS &RVcalculator, const scalar step, const SatParam &satParam, const scalar mass) {

        Eigen::Matrix<scalar, 6, 6 * N> A;

        const Containers::vector<typename RHS::State> initialTrajectory = Ballistics::NumericalMethods::integrate<Ballistics::NumericalMethods::RK4>(
                RVcalculator, initialState, time.back(), step, satParam, mass);



        for (indexType i = 0; i < 6; ++i) {
            typename RHS::State deltaState = calcDeltaState(initialState, delta[i], i);
            const Containers::vector<typename RHS::State> deltaTrajectory = Ballistics::NumericalMethods::integrate<Ballistics::NumericalMethods::RK4>(
                    RVcalculator, deltaState, time.back(), step, satParam, mass);

            A.row(i) =
        }
    }
}

#endif //BALLISTICS2023_DIFFCORRECTION_H
