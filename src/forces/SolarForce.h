//
// Created by ivankhripunov on 16.11.23.
//

#ifndef BALLISTICS2023_SOLARFORCE_H
#define BALLISTICS2023_SOLARFORCE_H

#include "Types/BasicTypes.h"

namespace Ballistics::Forces {

    template<typename SolarPressure, typename InteractionModel>
    class SolarForce {

        SolarPressure solarPressure_;
    public:

        [[nodiscard]] Vector3d calcForce(const Time <TT> &time,
                                         const Vector3d &position,
                                         const Vector3d &velocity,
                                         const Vector3d &sunPosition,
                                         const Vector3d &sunVelocity,
                                         const Vector3d &moonPosition,
                                         const InteractionModel &interactionModel) {

            const Vector3d solarPressure = solarPressure_.calcPressure(time, position, velocity, sunPosition,
                                                                       sunVelocity, moonPosition);

            return interactionModel.calcForce(solarPressure);
        };
    };
}

#endif //BALLISTICS2023_SOLARFORCE_H
