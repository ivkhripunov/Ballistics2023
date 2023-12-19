//
// Created by ivankhripunov on 19.12.23.
//

#ifndef BALLISTICS2023_DENSITYCALCULATOR_H
#define BALLISTICS2023_DENSITYCALCULATOR_H

#include "utility/interpolator/LinearInterpolator.h"

namespace Ballistics::Atmosphere {

    template<typename Interpolator>
    class DensityCalculator {

        Interpolator densityInterpolator_;

    public:
        DensityCalculator(const Interpolator &densityInterpolator) : densityInterpolator_(
                densityInterpolator) {};

        [[nodiscard]] scalar calcDensity (const scalar height) const noexcept {

            return densityInterpolator_.interpolate(height);
        }
    };
}

#endif //BALLISTICS2023_DENSITYCALCULATOR_H
