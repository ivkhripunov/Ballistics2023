//
// Created by ivankhripunov on 19.12.23.
//

#ifndef BALLISTICS2023_SOLARFORCE_H
#define BALLISTICS2023_SOLARFORCE_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"

namespace Ballistics::Force {

    template<typename SolarPressureCalculator>
    class SolarForce {

        SolarPressureCalculator solarPressureCalculator_;

    public:
        /**
         * Здесь по хорошему нужен шаблон от различных типов взаимодействий
         */
        struct SatParams {
            double solarArea;
        };

        SolarForce(const SolarPressureCalculator &solarPressureCalculator) : solarPressureCalculator_(
                solarPressureCalculator) {}

        template<typename InputParams>
        [[nodiscard]] Vector3d calcAcceleration(const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> &timeTT,
                                                const Vector3d &position,
                                                const Vector3d &velocity,
                                                const double mass,
                                                const SatParams satParams,
                                                const InputParams inputParams) const noexcept {

            return satParams.solarArea / mass *
                   solarPressureCalculator_.calcForcePerArea(timeTT, position, velocity, inputParams.sunPosition,
                                                             inputParams.sunVelocity, inputParams.moonPosition);
        }
    };

}

#endif //BALLISTICS2023_SOLARFORCE_H
