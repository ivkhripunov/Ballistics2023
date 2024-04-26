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

    template<typename RHS, typename SatParam>
    [[nodiscard]] DiffCorrectionOutput<RHS>
    diffCorrection(const Eigen::Matrix<scalar, 6, 6> &W, const typename RHS::IntegrationState &initialState,
                   const std::vector<Vector<scalar, 6>> &y, const std::array<scalar, 6> &delta,
                   const std::vector<TimeModule::Time<TimeModule::TimeScale::TT_SCALE>> &time,
                   const RHS &RVcalculator, const scalar step, const SatParam &satParam, const scalar mass,
                   const scalar tolerance, const indexType maxIter) {

        const indexType N = y.size();
        assert(N >= 1);
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

            Eigen::Matrix<scalar, 6, 6> ATWAsum = Eigen::Matrix<scalar, 6, 6>::Zero();
            Eigen::Vector<scalar, 6> ATWbsum = Eigen::Vector<scalar, 6>::Zero();
            scalar bTWb = 0;

            typename RHS::IntegrationState centralPoint = outputState;

            for (indexType i = 0; i < N; ++i) {

                centralPoint = Ballistics::NumericalMethods::integrate<Ballistics::NumericalMethods::RK4>(
                        RVcalculator, centralPoint, time[i], step, satParam, mass).back();

                const Vector<scalar, 6> b = y[i] - centralPoint.vector;

                bTWb += b.transpose() * W * b;

                Eigen::Matrix<scalar, 6, 6> A;
                for (indexType j = 0; j < 6; ++j) {
                    deltaPoints[j] = Ballistics::NumericalMethods::integrate<Ballistics::NumericalMethods::RK4>(
                            RVcalculator, deltaPoints[j], time[i], step, satParam, mass).back();

                    A.col(static_cast<Eigen::Index>(j)) = (deltaPoints[j].vector - centralPoint.vector) / delta[j];
                }

                const Eigen::Matrix<scalar, 6, 6> AT = A.transpose();
                const Eigen::Matrix<scalar, 6, 6> ATW = AT * W;
                const Eigen::Matrix<scalar, 6, 6> ATWA = ATW * A;
                ATWAsum += ATWA;

                const Eigen::Matrix<scalar, 6, 1> ATWb = ATW * b;
                ATWbsum += ATWb;
            }

            P = ATWAsum.inverse();

            if (bTWb / static_cast<scalar>(N) < tolerance) { return {outputState, P, true}; }

            const Vector<scalar, 6> deltaRV = P * ATWbsum;

            outputState.vector += deltaRV; // + или - ?
        }

        return {outputState, P, false};
    }
}

#endif //BALLISTICS2023_DIFFCORRECTION_H
