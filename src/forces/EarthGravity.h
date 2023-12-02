//
// Created by ivankhripunov on 19.10.23.
//

#ifndef BALLISTICS2023_CALCFORCES_H
#define BALLISTICS2023_CALCFORCES_H

#endif //BALLISTICS2023_CALCFORCES_H

#include "Eigen/Dense"
#include <GeographicLib/GravityModel.hpp>
#include "utility/types/BasicTypes.h"

namespace Ballistics::Force {

    class EarthGravityForce {

    private:
        GeographicLib::GravityModel gravityModel;

    public:
        EarthGravityForce(const std::string &path, const std::string &gravityModelName, int maxN, int maxM)
                : gravityModel(gravityModelName, path, maxN, maxM) {};

        [[nodiscard]] Vector3d calcForce(const Vector3d &positionECF);
    };

    void foo2();
}
