//
// Created by ivankhripunov on 16.11.23.
//

#ifndef BALLISTICS2023_SOLARRADIATION_H
#define BALLISTICS2023_SOLARRADIATION_H

#include "utility/types/BasicTypes.h"

namespace Ballistics::Force {

    class PenumbraRadiation {

        scalar sunRadius_;
        scalar bodyRadius_;

    public:
        PenumbraRadiation(const scalar sunRadius, const scalar bodyRadius) : sunRadius_(sunRadius),
                                                                             bodyRadius_(bodyRadius) {};

        [[nodiscard]] scalar calcShadow(const Vector3d &position, const Vector3d &bodyPosition,
                                                const Vector3d &sunPosition) const noexcept {}
    };

}

#endif //BALLISTICS2023_SOLARRADIATION_H
