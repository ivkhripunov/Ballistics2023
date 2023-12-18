//
// Created by ivankhripunov on 17.12.23.
//

#ifndef BALLISTICS2023_CELESTIALGRAVITY_H
#define BALLISTICS2023_CELESTIALGRAVITY_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"

namespace Ballistics::Force {

    class CelestialGravity {

        [[nodiscard]] Vector3d
        calcAccelerationSingleBody(const Vector3d &relativePosition, const double gravParameter) const noexcept {

            const double distanceSqr = relativePosition.squaredNorm();
            const double distance = std::sqrt(distanceSqr);
            const double distanceCube = distanceSqr * distance;

            return (-gravParameter / distanceCube) * relativePosition;
        }

    public:
        struct SatParams {
        };

        template<typename InputParams>
        [[nodiscard]] Vector3d calcAcceleration(const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> &timeTT,
                                                const Vector3d &position,
                                                const Vector3d &velocity,
                                                const double mass,
                                                const SatParams satParams,
                                                const InputParams inputParams) {

            return calcAccelerationSingleBody(position - inputParams.moonPosition, inputParams.moonGravParameter) +
                   calcAccelerationSingleBody(position - inputParams.jupiterPosition,
                                              inputParams.jupiterGravParameter) +
                   calcAccelerationSingleBody(position - inputParams.sunPosition, inputParams.sunGravParameter);
        }
    };

}

#endif //BALLISTICS2023_CELESTIALGRAVITY_H
