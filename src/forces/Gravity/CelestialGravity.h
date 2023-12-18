//
// Created by ivankhripunov on 17.12.23.
//

#ifndef BALLISTICS2023_CELESTIALGRAVITY_H
#define BALLISTICS2023_CELESTIALGRAVITY_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"

namespace Ballistics::Force {

    class CelestialGravity {

        double gravParameter_;

    public:
        struct SatParams {
        };

        CelestialGravity(const double gravParameter) noexcept: gravParameter_(gravParameter) {}

        [[nodiscard]] Vector3d
        calcAcceleration(const Ballistics::TimeModule::Time<TimeModule::TimeScale::TT_SCALE> &time,
                         const Vector3d &position,
                         const Vector3d &velocity,
                         const double mass,
                         const SatParams params) const noexcept {

            const double distanceSqr = position.squaredNorm();
            const double distance = std::sqrt(distanceSqr);
            const double distanceCube = distanceSqr * distance;

            return (-gravParameter_ / distanceCube) / mass * position;
        }
    };

}

#endif //BALLISTICS2023_CELESTIALGRAVITY_H
