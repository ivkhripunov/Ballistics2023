//
// Created by ivankhripunov on 08.02.24.
//

#ifndef BALLISTICS2023_RUNGEKUTTA_H
#define BALLISTICS2023_RUNGEKUTTA_H

#include <fstream>
#include "utility/types/BasicTypes.h"

namespace Ballistics::NumericalMethods {

    struct RK4 {
        static constexpr indexType stages = 4;
        static constexpr Containers::array<scalar, stages> column = {0, 0.5, 0.5, 1};
        static constexpr Containers::array<scalar, stages> row = {1. / 6, 1. / 3, 1. / 3, 1. / 6};
        static constexpr Containers::array<scalar, stages * (stages - 1) / 2> matrix = {0.5, 0, 0.5, 0, 0, 1};
    };

    struct IntegrationParameters {
        scalar step;
    };

    template<typename ButcherTable, typename RHS, typename SatParam>
    [[nodiscard]] typename RHS::IntegrationState integrateOneStep(const RHS &rhs,
                                                       const typename RHS::IntegrationState &initialIntegrationState,
                                                       const IntegrationParameters &integrationParameters,
                                                       SatParam &satParam, scalar mass) {

        const auto currentIntegrationState = initialIntegrationState;

        Containers::array<typename RHS::IntegrationVector, ButcherTable::stages> k;

        /* заполняем k */
        for (indexType i = 0; i < ButcherTable::stages; ++i) {
            typename RHS::IntegrationVector vec = RHS::IntegrationVector::Zero();
            //auto vec = RHS::IntegrationVector::Zero();
            for (indexType j = 0; j < i; ++j) {
                vec += ButcherTable::matrix[i * (i - 1) / 2 + j] * k[j];
            }

            vec *= integrationParameters.step;
            vec += currentIntegrationState.vector;

            k[i] = rhs.calc(
                    {vec, currentIntegrationState.argument + ButcherTable::column[i] * integrationParameters.step},
                    satParam, mass);

        }

        typename RHS::IntegrationVector result = RHS::IntegrationVector::Zero();

        for (indexType i = 0; i < ButcherTable::stages; ++i) {
            result += ButcherTable::row[i] * k[i];
        }

        result *= integrationParameters.step;

        result += currentIntegrationState.vector;

        const typename RHS::IntegrationState integrationState = {result,
                                                                 initialIntegrationState.argument + integrationParameters.step};

        return integrationState;
    }

    template<typename ButcherTable, typename RHS, typename SatParam>
    [[nodiscard]] Containers::vector<typename RHS::IntegrationState> integrate(const RHS &rhs,
                                                                    const typename RHS::IntegrationState &initialState,
                                                                    const decltype(RHS::IntegrationState::argument) &endPointArgument,
                                                                    const scalar step, SatParam &satParam,
                                                                    scalar mass) {

        const IntegrationParameters initialIntegrationParameters = {step};

        const auto size = static_cast<indexType>((endPointArgument - initialState.argument) / step) + 2;

        Containers::vector<typename RHS::IntegrationState> output;

        output.reserve(size);

        output.push_back(initialState);

        for (indexType i = 1; i < size - 1; ++i) {

            output.push_back(Ballistics::NumericalMethods::integrateOneStep<ButcherTable>(rhs, output[i - 1],
                                                                                          initialIntegrationParameters,
                                                                                          satParam, mass));
        }

        const IntegrationParameters lastIntervalIntegrationParameters = {endPointArgument - output[size - 2].argument};

        output.push_back(Ballistics::NumericalMethods::integrateOneStep<ButcherTable>(rhs, output[size - 2],
                                                                                      lastIntervalIntegrationParameters,
                                                                                      satParam,
                                                                                      mass));

        return output;
    }
}


#endif //BALLISTICS2023_RUNGEKUTTA_H;
