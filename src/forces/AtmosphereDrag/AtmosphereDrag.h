//
// Created by ivankhripunov on 19.12.23.
//

#ifndef BALLISTICS2023_ATMOSPHEREDRAG_H
#define BALLISTICS2023_ATMOSPHEREDRAG_H

#include "time/Time.h"
#include "atmosphere/DensityCalculator.h"


namespace Ballistics::Force {

    template<typename DensityCalculator>
    class AtmosphereDrag {

        double meanEarthRadius_ = 6.371e6;

        DensityCalculator densityCalculator_;

    public:
        struct SatParams {
            double dragCoeff_;
            double area_;
        };

        AtmosphereDrag(const DensityCalculator &densityCalculator) noexcept: densityCalculator_(
                densityCalculator) {};

        template<typename InputParams>
        [[nodiscard]] Vector3d calcAcceleration(const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> &timeTT,
                                                const Vector3d &position,
                                                const Vector3d &velocity,
                                                const double mass,
                                                const SatParams satParams,
                                                const InputParams inputParams) const noexcept {

            const double height = position.norm() - meanEarthRadius_;
            const double density = densityCalculator_.calcDensity(height);
            const double velocityNorm = velocity.norm();

            return -satParams.dragCoeff_ * density * velocityNorm * satParams.area_ / mass * velocity;
        }
    };

}


#endif //BALLISTICS2023_ATMOSPHEREDRAG_H
