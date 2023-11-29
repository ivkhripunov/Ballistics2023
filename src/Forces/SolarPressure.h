//
// Created by ivankhripunov on 16.11.23.
//

#ifndef BALLISTICS2023_SOLARPRESSURE_H
#define BALLISTICS2023_SOLARPRESSURE_H

#include "Types/Vector.h"

namespace Ballistics::Forces {

    template<typename TSImodel, typename ShadowModel>
    class SolarPressure {
        TSImodel tsiModel_;
        ShadowModel shadowModel_;

    public:
        SolarPressure(const TSImodel &tsiModel, const ShadowModel &shadowModel) : tsiModel_(tsiModel),
                                                                                  shadowModel_(shadowModel) {};

        /**
            Считает поток энергии, деленный на скорость света
         */
        [[nodiscard]] Vector3d
        calcSolarPressure(const Time <TT> &time,
                          const Vector3d &position,
                          const Vector3d &velocity,
                          const Vector3d &sunPosition,
                          const Vector3d &sunVelocity,
                          const Vector3d &moonPosition) const noexcept {
        }
    };
}

#endif //BALLISTICS2023_SOLARPRESSURE_H
