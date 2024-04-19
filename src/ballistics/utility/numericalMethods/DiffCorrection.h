//
// Created by ivankhripunov on 18.04.24.
//

#ifndef BALLISTICS2023_DIFFCORRECTION_H
#define BALLISTICS2023_DIFFCORRECTION_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"
#include "utility/numericalMethods/RungeKutta.h"

namespace Ballistics::NumericalMethods {

    template<typename IntegrationState>
    [[nodiscard]] IntegrationState
    calcDeltaState(const IntegrationState &initialState, const scalar delta, const indexType i) {

        IntegrationState result = initialState;

        result.vector(i) += delta;

        return result;
    }

    template<typename RHS>
    struct DiffCorrectionOutput {
        RHS::IntegrationState integrationState;
        Eigen::Matrix<scalar, 6, 6> P;

        bool converged;
    };

    template<indexType N, typename RHS, typename SatParam>
    [[nodiscard]] DiffCorrectionOutput<RHS>
    diffCorrectionStep(const Eigen::Matrix<scalar, 6, 6> &W, const typename RHS::IntegrationState &initialState,
                       const std::array<Vector<scalar, 6>, N> &y, const std::array<scalar, 6> &delta,
                       const std::array<TimeModule::Time<TimeModule::TimeScale::TT_SCALE>, N> &time,
                       const RHS &RVcalculator, const scalar step, const SatParam &satParam, const scalar mass,
                       const scalar tolerance, const indexType maxIter) {
        typename RHS::IntegrationState outputState = initialState;
        Eigen::Matrix<scalar, 6, 6> P;

        for (indexType iter = 0; iter < maxIter; ++iter) {
            Containers::array<typename RHS::IntegrationState, 6> deltaPoints = {
                    calcDeltaState(outputState, delta[0], 0),
                    calcDeltaState(outputState, delta[1], 1),
                    calcDeltaState(outputState, delta[2], 2),
                    calcDeltaState(outputState, delta[3], 3),
                    calcDeltaState(outputState, delta[4], 4),
                    calcDeltaState(outputState, delta[5], 5)};
            Eigen::Matrix<scalar, 6, 6> ATWAsum;
            Eigen::Vector<scalar, 6> ATWbsum;
            scalar bTWb = 0;

            typename RHS::IntegrationState point = initialState;

            for (indexType i = 0; i < N; ++i) {

                point = Ballistics::NumericalMethods::integrate<Ballistics::NumericalMethods::RK4>(
                        RVcalculator, point, time[i], step, satParam, mass).back();

                const Vector<scalar, 6> b = y[i] - point.vector;

                bTWb += b.transpose() * W * b;

                Eigen::Matrix<scalar, 6, 6> A;
                for (indexType j = 0; j < 6; ++j) {
                    deltaPoints[j] = Ballistics::NumericalMethods::integrate<Ballistics::NumericalMethods::RK4>(
                            RVcalculator, deltaPoints[j], time[i], step, satParam, mass).back();

                    A.row(j) = (y[j] - deltaPoints[j].vector) / delta[j];
                }

                const Eigen::Matrix<scalar, 6, 6> AT = A.transpose();
                const Eigen::Matrix<scalar, 6, 6> ATW = AT * W;
                const Eigen::Matrix<scalar, 6, 6> ATWA = ATW * A;
                ATWAsum += ATWA;

                const Eigen::Matrix<scalar, 6, 1> ATWb = ATW * b;
                ATWbsum += ATWb;
            }

            P = ATWAsum.inverse();

            const Vector<scalar, 6> deltaRV = P * ATWbsum;

            outputState.vector += deltaRV;

            if (std::sqrt(bTWb / N) < tolerance) { return {outputState, P, true}; }
        }

        return {outputState, P, false};
    }
}

#endif //BALLISTICS2023_DIFFCORRECTION_H
