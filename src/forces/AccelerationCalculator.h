//
// Created by ivankhripunov on 18.12.23.
//

#ifndef BALLISTICS2023_ACCELERATIONCALCULATOR_H
#define BALLISTICS2023_ACCELERATIONCALCULATOR_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"

namespace Ballistics::Force {

    template<typename TimeConverter, typename FrameConverter, typename EphemerisCalculator, typename EarthGravity, typename ... OtherForces>
    class AccelerationCalculator {

        TimeConverter timeConverter_;
        FrameConverter frameConverter_;
        EphemerisCalculator ephemerisCalculator_;

        EarthGravity earthGravity_;
        std::tuple<OtherForces...> otherForces_;

        struct SatParams : EarthGravity::SatParams, OtherForces::SatParams ... {
        };

        struct InputParams {
            Vector3d moonPosition;
            Vector3d jupiterPosition;
            Vector3d sunPosition;

            double moonGravParameter;
            double jupiterGravParameter;
            double sunGravParameter;

            TimeModule::Time<TimeModule::TimeScale::TDB_SCALE> tdbTime;
        };

        [[nodiscard]] InputParams
        calcInputParam(const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> timeTT) const noexcept {
            const TimeModule::Time<TimeModule::TimeScale::TDB_SCALE> timeTDB = timeConverter_.convert<TimeModule::TimeScale::TDB_SCALE>(
                    timeTT);

            const int centerBody = 3;
            const Vector3d moonPosition = ephemerisCalculator_.calculateBody<EphemerisCalculator::CalcType::POS>(10,
                                                                                                                 centerBody,
                                                                                                                 timeTDB);
            const Vector3d jupiterPosition = ephemerisCalculator_.calculateBody<EphemerisCalculator::CalcType::POS>(5,
                                                                                                                    centerBody,
                                                                                                                    timeTDB);
            const Vector3d sunPosition = ephemerisCalculator_.calculateBody<EphemerisCalculator::CalcType::POS>(11,
                                                                                                                centerBody,
                                                                                                                timeTDB);

            const double muMoon = ephemerisCalculator_.calcGravParameter(10);
            const double muJupiter = ephemerisCalculator_.calcGravParameter(5);
            const double muSun = ephemerisCalculator_.calcGravParameter(11);

            const AccelerationCalculator::InputParams inputParams = {moonPosition,
                                                                     jupiterPosition,
                                                                     sunPosition,
                                                                     muMoon,
                                                                     muJupiter,
                                                                     muSun,
                                                                     timeTDB};
        }

    public:

        AccelerationCalculator(const TimeConverter &timeConverter, const FrameConverter &frameConverter,
                               const EphemerisCalculator &ephemerisCalculator, const EarthGravity &earthGravity,
                               const std::tuple<OtherForces...> &otherForces) noexcept: timeConverter_(timeConverter),
                                                                                        frameConverter_(frameConverter),
                                                                                        ephemerisCalculator_(
                                                                                                ephemerisCalculator),
                                                                                        earthGravity_(earthGravity),
                                                                                        otherForces_(otherForces) {};

        [[nodiscard]]
        Vector3d calcAcceleration(
                const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> timeTT,
                const Vector3d &position,
                const Vector3d &velocity,
                double mass,
                const SatParams &satParams
        ) const noexcept {

            const InputParams inputParams = calcInputParam(timeTT);

            const auto sum = [&timeTT, &position, &velocity, mass, &satParams, &inputParams](
                    const auto &...forces) {
                if constexpr (std::tuple_size_v<OtherForces...> != 0) {
                    return (forces.calcAcceleration(timeTT, position, velocity, mass, satParams, inputParams) + ...);
                } else {
                    return Vector3d::Zero();
                }
            };

            const Quaternion<double> quaternionGCRStoITRS = frameConverter_.quaternionGCRStoITRS(timeTT);
            const Vector3d positionITRS = quaternionGCRStoITRS._transformVector(position);
            const Vector3d earthGravAccelerationITRS = earthGravity_.calcAcceleration(positionITRS);
            const Vector3d earthGravAccelerationGCRS = (quaternionGCRStoITRS.inverse())._transformVector(
                    earthGravAccelerationITRS);

            return earthGravAccelerationGCRS + std::apply(sum, otherForces_);

        }
    };
}

#endif //BALLISTICS2023_ACCELERATIONCALCULATOR_H
