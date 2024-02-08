//
// Created by ivankhripunov on 08.02.24.
//

#ifndef BALLISTICS2023_RUNGEKUTTA_H
#define BALLISTICS2023_RUNGEKUTTA_H

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

    template<typename ButcherTable, typename RHS>
    [[nodiscard]] typename RHS::State integrateOneStep(const RHS &rhs,
                                                       const typename RHS::State &initialState,
                                                       const IntegrationParameters &integrationParameters) {

        const auto currentIntegrationState = rhs.toIntegrationState(initialState);

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
                    {vec, currentIntegrationState.argument + ButcherTable::column[i] * integrationParameters.step});

        }

        typename RHS::IntegrationVector result = RHS::IntegrationVector::Zero();
        //auto result = RHS::IntegrationVector::Zero();

        for (indexType i = 0; i < ButcherTable::stages; ++i) {
            result += ButcherTable::row[i] * k[i];
        }

        result *= integrationParameters.step;

        result += currentIntegrationState.vector;

        const typename RHS::IntegrationState integrationState = {result, initialState.argument + integrationParameters.step};

        return rhs.toState(integrationState);
    }
}

#endif //BALLISTICS2023_RUNGEKUTTA_H;
