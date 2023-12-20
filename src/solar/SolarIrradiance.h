//
// Created by ivankhripunov on 16.11.23.
//

#ifndef BALLISTICS2023_SOLARIRRADIANCE_H
#define BALLISTICS2023_SOLARIRRADIANCE_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"

namespace Ballistics::Solar {

    class ConstantTSI {
        static constexpr scalar TSI_ = static_cast<scalar>(1366);

    public:
        [[nodiscard]] static constexpr scalar
        calcTSI(const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> & /*tt*/) noexcept { return TSI_; }
    };
}

#endif //BALLISTICS2023_SOLARIRRADIANCE_H
