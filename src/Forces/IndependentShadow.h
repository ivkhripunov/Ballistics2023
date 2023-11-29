//
// Created by ivankhripunov on 16.11.23.
//

#ifndef BALLISTICS2023_INDEPENDENTSHADOW_H
#define BALLISTICS2023_INDEPENDENTSHADOW_H

#include "Types/Vector.h"

namespace Ballistics::Force {

    template<typename EarthShadow, typename MoonShadow>
    class independentShadow {
        EarthShadow earthShadow_;
        MoonShadow moonShadow_;

    public:
        independentShadow(const EarthShadow &earthShadow, const MoonShadow &moonShadow) : earthShadow_(earthShadow),
                                                                                          moonShadow_(moonShadow) {}

        [[nodiscard]] Vector3d
        calcFlux(const Vector3d &position,
                 const Vector3d &sunPosition,
                 const Vector3d &moonPosition,
                 const Vector3d &zeroFlux) const {

            const scalar f = std::min(earthShadow_.calcShadow(position, sunPosition, Vector3d::Zero()),
                                      moonShadow_.calcShadow(position, sunPosition, moonPosition));

            return zeroFlux * f;
        }
    };
}

#endif //BALLISTICS2023_INDEPENDENTSHADOW_H
