//
// Created by ivankhripunov on 08.02.24.
//

#ifndef BALLISTICS2023_CENTRALGRAVITY_H
#define BALLISTICS2023_CENTRALGRAVITY_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"

namespace Ballistics::RHS {

    class CentralGravity {

        static constexpr scalar gravParameter = 3.986e14;

    public:

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

        [[nodiscard]] inline IntegrationVector calc(const IntegrationState &integrationState) const noexcept {

            const Vector3d pos = integrationState.vector.segment<3>(0);
            const scalar radiusSqr = pos.squaredNorm();
            const scalar radius = std::sqrt(radiusSqr);
            const Vector3d acceleration = -gravParameter / (radius * radiusSqr) * pos;

            IntegrationVector result;

            result.segment<3>(0) = integrationState.vector.segment<3>(3);
            result.segment<3>(3) = acceleration;

            return result;
        }

        [[nodiscard]] IntegrationState toIntegrationState(const State &state) const noexcept {

            IntegrationVector integrationVector;
            integrationVector.segment<3>(0) = state.position;
            integrationVector.segment<3>(3) = state.velocity;

            return {integrationVector, state.argument};
        }

        [[nodiscard]] State toState(const IntegrationState &integrationState) const noexcept {

            const Vector3d position = integrationState.vector.segment<3>(0);
            const Vector3d velocity = integrationState.vector.segment<3>(3);

            return {position, velocity, integrationState.argument};
        }

    };
}

#endif //BALLISTICS2023_CENTRALGRAVITY_H
