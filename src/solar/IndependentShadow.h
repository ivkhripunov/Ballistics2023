//
// Created by ivankhripunov on 16.11.23.
//

#ifndef BALLISTICS2023_INDEPENDENTSHADOW_H
#define BALLISTICS2023_INDEPENDENTSHADOW_H

#include "utility/types/BasicTypes.h"

namespace Ballistics::Solar {

    template<typename EarthShadow, typename MoonShadow>
    class IndependentShadow {
        EarthShadow earthShadow_;
        MoonShadow moonShadow_;

    public:
        IndependentShadow(const EarthShadow &earthShadow, const MoonShadow &moonShadow) noexcept : earthShadow_(earthShadow),
                                                                                          moonShadow_(moonShadow) {}

        [[nodiscard]] Vector3d
        calcFlux(const Vector3d &position,
                 const Vector3d &sunPosition,
                 const Vector3d &moonPosition,
                 const Vector3d &zeroFlux) const noexcept {

            const double earthShadowFactor = earthShadow_.calcShadowFunction(position, sunPosition, Vector3d::Zero());
            const double moonShadowFactor = moonShadow_.calcShadowFunction(position, sunPosition, moonPosition);

            const scalar resultShadowFactor = std::min(earthShadowFactor, moonShadowFactor);

            return zeroFlux * resultShadowFactor;
        }
    };
}

#endif //BALLISTICS2023_INDEPENDENTSHADOW_H
