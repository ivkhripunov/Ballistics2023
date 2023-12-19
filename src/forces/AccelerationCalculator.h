//
// Created by ivankhripunov on 18.12.23.
//

#ifndef BALLISTICS2023_ACCELERATIONCALCULATOR_H
#define BALLISTICS2023_ACCELERATIONCALCULATOR_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"

//TODO fix condition

namespace Ballistics::Force {

    template<typename TimeConverter, typename FrameConverter, typename EphemerisCalculator, typename EarthGravity, typename ... OtherForces>
    class AccelerationCalculator {

        TimeConverter timeConverter_;
        FrameConverter frameConverter_;
        EphemerisCalculator ephemerisCalculator_;

        struct InputParams {
            Vector3d moonPosition;
            Vector3d jupiterPosition;
            Vector3d sunPosition;

            Vector3d sunVelocity;

            double moonGravParameter;
            double jupiterGravParameter;
            double sunGravParameter;

            TimeModule::Time<TimeModule::TimeScale::TDB_SCALE> tdbTime;
        };

        [[nodiscard]] InputParams
        calcInputParam(const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> timeTT) const noexcept {

            const TimeModule::Time<TimeModule::TimeScale::TDB_SCALE> timeTDB = timeConverter_.convertTT_TDB(
                    timeTT);

            const auto stdToVector3d = [](const Containers::vector<double> &vector3) {
                return Vector3d{vector3[0], vector3[1], vector3[2]};
            };

            const int centerBody = 3;
            const Vector3d moonPosition = stdToVector3d(ephemerisCalculator_.calculateBodyR(10, centerBody, timeTDB));

            const Vector3d jupiterPosition = stdToVector3d(ephemerisCalculator_.calculateBodyR(5, centerBody,
                                                                                               timeTDB));

            const Containers::vector<double> sunRV = ephemerisCalculator_.calculateBodyRV(11, centerBody, timeTDB);

            const Vector3d sunPosition = {sunRV[0], sunRV[1], sunRV[2]};
            const Vector3d sunVelocity = {sunRV[3], sunRV[4], sunRV[5]};

            const double muMoon = ephemerisCalculator_.calcGravParameter(10);
            const double muJupiter = ephemerisCalculator_.calcGravParameter(5);
            const double muSun = ephemerisCalculator_.calcGravParameter(11);

            return {moonPosition,
                    jupiterPosition,
                    sunPosition,
                    sunVelocity,
                    muMoon,
                    muJupiter,
                    muSun,
                    timeTDB};
        }

    public:

        struct allSatParams : OtherForces::SatParams ... {
        };

        AccelerationCalculator(const TimeConverter &timeConverter, const FrameConverter &frameConverter,
                               const EphemerisCalculator &ephemerisCalculator, EarthGravity &,
                               const std::tuple<OtherForces...> &) noexcept: timeConverter_(timeConverter),
                                                                             frameConverter_(
                                                                                     frameConverter),
                                                                             ephemerisCalculator_(
                                                                                     ephemerisCalculator) {};

        [[nodiscard]]
        Vector3d calcAcceleration(const EarthGravity &earthGravity,
                                  const std::tuple<OtherForces...> &otherForces,
                                  const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> timeTT,
                                  const Vector3d &position,
                                  const Vector3d &velocity,
                                  double mass,
                                  const allSatParams &allSatParams
        ) const noexcept {

            const InputParams inputParams = calcInputParam(timeTT);

            const auto sum = [&timeTT, &position, &velocity, mass, &allSatParams, &inputParams](
                    const auto &...forces) {
                if constexpr (/*std::tuple_size_v<OtherForces...> != 0*/ true) {
                    return (forces.calcAcceleration(timeTT, position, velocity, mass, allSatParams, inputParams) + ...);
                } else {
                    return Vector3d::Zero();
                }
            };

            const Quaternion<double> quaternionGCRStoITRS = frameConverter_.quaternionGCRStoITRS(timeTT);
            const Vector3d positionITRS = quaternionGCRStoITRS._transformVector(position);
            const Vector3d earthGravAccelerationITRS = earthGravity.calcAccelerationECEF(positionITRS);
            const Vector3d earthGravAccelerationGCRS = (quaternionGCRStoITRS.inverse())._transformVector(
                    earthGravAccelerationITRS);

            return earthGravAccelerationGCRS + std::apply(sum, otherForces);
        }
    };
}

#endif //BALLISTICS2023_ACCELERATIONCALCULATOR_H
