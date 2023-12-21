//
// Created by ivankhripunov on 18.12.23.
//

#ifndef BALLISTICS2023_ACCELERATIONCALCULATOR_H
#define BALLISTICS2023_ACCELERATIONCALCULATOR_H

#include "ballistics/utility/types/BasicTypes.h"
#include "ballistics/time/Time.h"

//TODO fix condition

namespace Ballistics::Force {

    template<typename TimeConverter, typename FrameConverter, typename EphemerisCalculator, typename EarthGravity, typename ... OtherForces>
    class AccelerationCalculator {

    public:

        struct allSatParams : OtherForces::SatParams ... {
        };

    private:

        TimeConverter timeConverter_;
        FrameConverter frameConverter_;
        EphemerisCalculator ephemerisCalculator_;

        struct InputParams {
            Vector3d moonPositionGCRS;
            Vector3d jupiterPositionGCRS;
            Vector3d sunPositionGCRS;
            Vector3d sunVelocityGCRS;

            double moonGravParameter;
            double jupiterGravParameter;
            double sunGravParameter;
        };

        [[nodiscard]] InputParams
        calcInputParam(const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> timeTT,
                       const Quaternion<double> &quaternionGCRStoITRS) const noexcept {

            const TimeModule::Time<TimeModule::TimeScale::TDB_SCALE> timeTDB = timeConverter_.convertTT_TDB(
                    timeTT);

            const auto stdToVector3d = [](const Containers::vector<double> &vector3) {
                return Vector3d{vector3[0], vector3[1], vector3[2]};
            };

            const int centerBody = 3;
            const Vector3d moonPositionGCRS = stdToVector3d(
                    ephemerisCalculator_.calculateBodyR(10, centerBody, timeTDB));

            const Vector3d jupiterPositionGCRS = stdToVector3d(ephemerisCalculator_.calculateBodyR(5, centerBody,
                                                                                                   timeTDB));

            const Containers::vector<double> sunRVgcrs = ephemerisCalculator_.calculateBodyRV(11, centerBody, timeTDB);

            const Vector3d sunPositionGCRS = {sunRVgcrs[0], sunRVgcrs[1], sunRVgcrs[2]};
            const Vector3d sunVelocityGCRS = {sunRVgcrs[3], sunRVgcrs[4], sunRVgcrs[5]};

            const Vector3d moonPositionITRS = quaternionGCRStoITRS._transformVector(moonPositionGCRS);
            const Vector3d jupiterPositionITRS = quaternionGCRStoITRS._transformVector(jupiterPositionGCRS);
            const Vector3d sunPositionITRS = quaternionGCRStoITRS._transformVector(sunPositionGCRS);
            const Vector3d sunVelocityITRS = quaternionGCRStoITRS._transformVector(sunVelocityGCRS);

            const double muMoon = ephemerisCalculator_.calcGravParameter(10);
            const double muJupiter = ephemerisCalculator_.calcGravParameter(5);
            const double muSun = ephemerisCalculator_.calcGravParameter(11);

            return {moonPositionITRS,
                    jupiterPositionITRS,
                    sunPositionITRS,
                    sunVelocityITRS,
                    muMoon,
                    muJupiter,
                    muSun};
        }

        template<unsigned int ...Is>
        [[nodiscard]] Vector3d auxFunction(const std::tuple<OtherForces...> &otherForces,
                                           const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> timeTT,
                                           const Vector3d &positionGCRS,
                                           const Vector3d &velocityGCRS,
                                           double mass,
                                           const allSatParams &allSatParams,
                                           const InputParams &inputParams,
                                           std::integer_sequence<unsigned int, Is...>) const noexcept {

            return (std::get<Is>(otherForces).calcAcceleration(timeTT, positionGCRS, velocityGCRS, mass, allSatParams,
                                                               inputParams) + ...);
        }


    public:

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
                                  const Vector3d &positionITRS,
                                  const Vector3d &velocityITRS,
                                  double mass,
                                  const allSatParams &allSatParams
        ) const noexcept {

            const Quaternion<double> quaternionGCRStoITRS = frameConverter_.quaternionGCRStoITRS(timeTT);

            const InputParams inputParams = calcInputParam(timeTT, quaternionGCRStoITRS);

            const Vector3d earthGravAccelerationITRS = earthGravity.calcAccelerationECEF(positionITRS);

            const Vector3d otherForcesAcceleration = auxFunction(otherForces, timeTT, positionITRS, velocityITRS, mass, allSatParams, inputParams,
                                                                 std::make_integer_sequence<unsigned int, sizeof...(OtherForces)>{});

            return earthGravAccelerationITRS + otherForcesAcceleration;
        }
    };
}

#endif //BALLISTICS2023_ACCELERATIONCALCULATOR_H
