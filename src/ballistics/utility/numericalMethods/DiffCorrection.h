//
// Created by ivankhripunov on 18.04.24.
//

#ifndef BALLISTICS2023_DIFFCORRECTION_H
#define BALLISTICS2023_DIFFCORRECTION_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"

namespace Ballistics::NumericalMethods {

    [[nodiscard]] Vector<scalar, 6> convertFromRV(const Vector<scalar, 6> &RV) {
        return RV;
    }

    template<indexType N, typename AccelerationCalculator>
    [[nodiscard]] Vector<scalar, 6>
    diffCorrection(const Eigen::Matrix<scalar, 6, 6> &W, const Vector<scalar, 6> &initialGuess,
                   const std::array<Vector<scalar, 6>, N> &y, const Vector<scalar, 6> &delta,
                   const std::array<TimeModule::Time<TimeModule::TimeScale::TT_SCALE>, N> &time,
                   const AccelerationCalculator &accelerationCalculator) {


    }
}

#endif //BALLISTICS2023_DIFFCORRECTION_H
