//
// Created by ivankhripunov on 19.10.23.
//

#include "EarthGravity.h"

namespace Ballistics::Force {

    Vector3d EarthGravityForce::calcAcceleration(const Ballistics::Vector3d &positionECEF) const noexcept {
        double gx = 0;
        double gy = 0;
        double gz = 0;
        gravityModel.V(positionECEF.x(), positionECEF.y(), positionECEF.z(), gx, gy, gz);

        return {gx, gy, gz};
    }
}

