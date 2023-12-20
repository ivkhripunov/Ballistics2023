//
// Created by ivankhripunov on 19.12.23.
//

#ifndef BALLISTICS2023_SOLARPRESSURE_H
#define BALLISTICS2023_SOLARPRESSURE_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"

namespace Ballistics::Solar {

    template<typename IndependentShadow, typename TSImodel>
    class SolarPressureCalculator {

        IndependentShadow independentShadow_;
        TSImodel TSImodel_;

        constexpr static double AU = 149597870700;
        constexpr static double c = 3e8;

        [[nodiscard]] Vector3d
        calcJ0(const TimeModule::Time <TimeModule::TimeScale::TT_SCALE> &timeTT, const Vector3d &satPosition,
               const Vector3d &satVelocity,
               const Vector3d &sunPosition, const Vector3d &sunVelocity) const noexcept {

            const Vector3d relativeVector = (satPosition - sunPosition);
            const double relativeDistanceSq = relativeVector.squaredNorm();
            const double relativeDistance = std::sqrt(relativeDistanceSq);

            const Vector3d n = relativeVector / relativeDistance;

            const Vector3d relativeVelocityDivC = (satVelocity - sunVelocity) / c;

            const Vector3d direction = n * (1 - n.dot(relativeVelocityDivC)) - relativeVelocityDivC;

            const Vector3d j0 = TSImodel_.calcTSI(timeTT) * AU * AU / relativeDistanceSq * direction;

            return j0;
        }

    public:

        SolarPressureCalculator(const IndependentShadow &independentShadow, const TSImodel &tsImodel)
                : independentShadow_(independentShadow),
                  TSImodel_(tsImodel) {};

        [[nodiscard]] Vector3d calcForcePerArea(const TimeModule::Time <TimeModule::TimeScale::TT_SCALE> &timeTT,
                                                const Vector3d &satPosition,
                                                const Vector3d &satVelocity,
                                                const Vector3d &sunPosition,
                                                const Vector3d &sunVelocity,
                                                const Vector3d &moonPosition
        ) const noexcept {

            const Vector3d j0 = calcJ0(timeTT, satPosition, satVelocity, sunPosition, sunVelocity);
            const double j0norm = j0.norm();
            const Vector3d j0normalized = j0 / j0norm;

            return j0norm *
                   independentShadow_.calcFlux(satPosition, sunPosition, moonPosition, j0normalized) / c;
        }
    };
}

#endif //BALLISTICS2023_SOLARPRESSURE_H
