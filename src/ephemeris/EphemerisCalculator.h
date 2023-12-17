//
// Created by ivankhripunov on 17.12.23.
//

#ifndef BALLISTICS2023_EPHEMERISCALCULATOR_H
#define BALLISTICS2023_EPHEMERISCALCULATOR_H

#include "../third_party/calceph/calceph.h"
#include "utility/types/BasicTypes.h"
#include "exceptions/TimeExceptions.h"
#include "time/Time.h"
#include "CelestialBodies.h"

namespace Ballistics::Ephemeris {

    class EphemerisCalculator {
    private:
        double AU;

    public:

        enum class CalcType {
            POS = 3,
            POSVEL = 6
        };

        EphemerisCalculator(const Containers::string &path) {
            const int statusOpen = calceph_sopen(path.c_str());

            if (statusOpen != 1) {
                throw Ballistics::Exceptions::TimeModuleException("EPHEMERIS OPEN ERROR");
            }

            const int statusConstant = calceph_sgetconstant("AU", &AU);

            if (statusConstant == 0) {
                throw Ballistics::Exceptions::TimeModuleException("EPHEMERIS CONSTANT NOT FOUND");
            }
        }

        template<CalcType calcType>
        [[nodiscard]] Containers::vector<scalar>
        calculateBody(const int targetBody, const int centerBody,
                      const Ballistics::TimeModule::Time<TimeModule::TimeScale::TDB_SCALE> &time) const {

            double PV[6];

            const int jdInt = static_cast<int>(time.jdDayInt());
            const int jdFrac = static_cast<int>(time.jdDayFrac());

            const int status = calceph_scompute(jdInt, jdFrac, targetBody, centerBody, PV);

            if (status != 1) {
                throw Ballistics::Exceptions::TimeModuleException("EPHEMERIS CALC ERROR");
            }

            const double AUe3 = AU * 1e3;
            const double AUe3divDay = AUe3 / 86400;
            const auto auToMeters = [&](const double distance) { return distance * AUe3; };
            const auto auPerDayToMetersPerSecond = [&](const double speed) { return speed * AUe3divDay; };

            switch (calcType) {

                case CalcType::POS:
                    return {static_cast<scalar>(auToMeters(PV[0])),
                            static_cast<scalar>(auToMeters(PV[1])),
                            static_cast<scalar>(auToMeters(PV[2]))};

                default:
                    return {static_cast<scalar>(auToMeters(PV[0])),
                            static_cast<scalar>(auToMeters(PV[1])),
                            static_cast<scalar>(auToMeters(PV[2])),
                            static_cast<scalar>(auPerDayToMetersPerSecond(PV[0])),
                            static_cast<scalar>(auPerDayToMetersPerSecond(PV[1])),
                            static_cast<scalar>(auPerDayToMetersPerSecond(PV[2]))};
            }

        }
    };
}

#endif //BALLISTICS2023_EPHEMERISCALCULATOR_H
