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
    [[nodiscard]] typename RHS::State integrateOneStep(const RHS &rhs,
                                                       const typename RHS::State &initialState,
                                                       const IntegrationParameters &integrationParameters,
                                                       SatParam &satParam, scalar mass) {

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
                                                                 initialState.argument + integrationParameters.step};

        return rhs.toState(integrationState);
    }

    template<typename ButcherTable, typename RHS, typename SatParam>
    [[nodiscard]] typename RHS::State integrate(const RHS &rhs,
                                                const typename RHS::State &initialState,
                                                const decltype(RHS::State::argument) &endPointArgument,
                                                const scalar step, SatParam &satParam, scalar mass) {

        const std::string currentFile = __FILE__;
        const std::string outputPath = "/home/ivankhripunov/CLionProjects/ballistics2023/output/RK_INTERVAL";

        std::ofstream myfile(outputPath, std::ios::trunc);

        const IntegrationParameters initialIntegrationParameters = {step};

        typename RHS::State result = initialState;
        while (result.argument < endPointArgument - step) {

            result = Ballistics::NumericalMethods::integrateOneStep<ButcherTable>(rhs, result,
                                                                                  initialIntegrationParameters,
                                                                                  satParam, mass);

            myfile << result.position[0] << " " << result.position[1] << " " << result.position[2] << " " << result.position.norm() - 6371e3 << std::endl;
        }

        const IntegrationParameters lastIntervalIntegrationParameters = {endPointArgument - result.argument};

        return Ballistics::NumericalMethods::integrateOneStep<ButcherTable>(rhs, result,
                                                                            lastIntervalIntegrationParameters, satParam,
                                                                            mass);
    }
}


#endif //BALLISTICS2023_RUNGEKUTTA_H;
