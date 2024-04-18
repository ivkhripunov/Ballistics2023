//
// Created by ivankhripunov on 28.03.24.
//

#ifndef BALLISTICS2023_RV_H
#define BALLISTICS2023_RV_H

#include "ballistics/utility/types/BasicTypes.h"
#include "ballistics/time/Time.h"
#include "ballistics/forces/AccelerationCalculator.h"

namespace Ballistics::RHS {

    template<typename AccelerationCalculator>
    class RV {

        AccelerationCalculator accelerationCalculator_;

    public:

        RV(const AccelerationCalculator &accelerationCalculator) : accelerationCalculator_(accelerationCalculator) {};

        using ArgumentType = TimeModule::Time<TimeModule::TimeScale::TT_SCALE>;

        using IntegrationVector = Vector<scalar, 6>;

        struct IntegrationState {
            IntegrationVector vector;
            TimeModule::Time<TimeModule::TimeScale::TT_SCALE> argument;
        };

        struct State {
            Vector3d position;
            Vector3d velocity;

            TimeModule::Time<TimeModule::TimeScale::TT_SCALE> argument;
        };

        [[nodiscard]] inline IntegrationVector
        calc(const IntegrationState &integrationState, const AccelerationCalculator::allSatParams &satParams,
             const Ballistics::scalar mass) const noexcept {

            const Vector3d position = integrationState.vector.template segment<3>(0);
            const Vector3d velocity = integrationState.vector.template segment<3>(3);
            const Vector3d acceleration = accelerationCalculator_.calcAcceleration(integrationState.argument, position,
                                                                                   velocity, satParams, mass);

            IntegrationVector result;

            result.segment<3>(0) = integrationState.vector.segment<3>(3);
            result.segment<3>(3) = acceleration;

            return result;
        }

        [[nodiscard]] inline static IntegrationState toIntegrationState(const State &state) noexcept {

            IntegrationVector integrationVector;
            integrationVector.segment<3>(0) = state.position;
            integrationVector.segment<3>(3) = state.velocity;

            return {integrationVector, state.argument};
        }

        [[nodiscard]] inline static State toState(const IntegrationState &integrationState) noexcept {

            const Vector3d position = integrationState.vector.segment<3>(0);
            const Vector3d velocity = integrationState.vector.segment<3>(3);

            return {position, velocity, integrationState.argument};
        }

    };
}

#endif //BALLISTICS2023_RV_H
