//
// Created by ivankhripunov on 19.12.23.
//

#ifndef BALLISTICS2023_SHADOWMODEL_H
#define BALLISTICS2023_SHADOWMODEL_H

#include "utility/types/BasicTypes.h"

namespace Ballistics::Solar {

    class PenumbraModel {

        double sunRadius_;
        double bodyRadius_;

    public:

        PenumbraModel(const double sunRadius, const double bodyRadius) noexcept: sunRadius_(sunRadius),
                                                                                 bodyRadius_(bodyRadius) {};

        [[nodiscard]] double calcShadowFunction(const Vector3d &satPosition, const Vector3d &sunPosition,
                                                const Vector3d &celestialBodyPosition) const noexcept {

            const Vector3d relativeSatSunPosition = sunPosition - satPosition;
            const Vector3d relativeSatBodyPosition = celestialBodyPosition - satPosition;

            const double relativeSatSunPositionNorm = relativeSatSunPosition.norm();
            const double relativeSatBodyPositionNorm = relativeSatBodyPosition.norm();

            const double thetaBody = std::asin(bodyRadius_ / relativeSatBodyPositionNorm);
            const double thetaSun = std::asin(sunRadius_ / relativeSatSunPositionNorm);
            const double thetaBodySun = std::acos(
                    relativeSatBodyPosition.dot(relativeSatSunPosition) / (relativeSatSunPositionNorm *
                                                                           relativeSatBodyPositionNorm));

            const double thetaBodySq = thetaBody * thetaBody;
            const double thetaSunSq = thetaSun * thetaSun;
            const double thetaBodySunSq = thetaBodySun * thetaBodySun;

            const double cafAngle = std::acos(
                    (thetaSunSq + thetaBodySunSq - thetaBodySq) / (2 * thetaBodySun * thetaSun));
            const double cbdAngle = std::acos(
                    (thetaBodySq + thetaBodySunSq - thetaSunSq) / (2 * thetaBodySun * thetaBody));

            const double areaAFC = thetaSunSq * cafAngle / 2;
            const double areaAEC = thetaSunSq * std::sin(2 * cafAngle) / 4;
            const double areaBDC = thetaBodySq * cbdAngle / 2;
            const double areaBEC = thetaBodySq * std::sin(2 * cbdAngle) / 4;

            const double intersectionArea = 2 * (areaAFC - areaAEC + areaBDC - areaBEC);

            return 1 - intersectionArea / (M_PI * thetaSunSq);
        }
    };
}

#endif //BALLISTICS2023_SHADOWMODEL_H
