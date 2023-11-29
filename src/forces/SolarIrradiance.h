//
// Created by ivankhripunov on 16.11.23.
//

#ifndef BALLISTICS2023_SOLARIRRADIANCE_H
#define BALLISTICS2023_SOLARIRRADIANCE_H

#include "Types/BasicTypes.h"

namespace Ballistics::Forces {

    class ConstantTSI {
        static constexpr scalar TSI_ = 1366;

    public:
        [[nodiscard]] static constexpr scalar TSI(const Time <TT> & /*tt*/) noexcept { return TSI_; }
    };
}

#endif //BALLISTICS2023_SOLARIRRADIANCE_H
