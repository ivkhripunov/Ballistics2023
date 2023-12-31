//
// Created by ivankhripunov on 30.11.23.
//

#ifndef BALLISTICS2023_DUTCORRECTION_H
#define BALLISTICS2023_DUTCORRECTION_H

#include "Time.h"
#include "ballistics/utility/interpolator/LinearInterpolator.h"

//TODO: add leap seconds control

namespace Ballistics::TimeModule {

    class DutContainer {

    private:
        Utility::Interpolator<scalar, scalar> interpolator;

    public:

        DutContainer(const Containers::vector<scalar> &timePointsMJD, const Containers::vector<scalar> &dut) noexcept {

            Containers::vector<Utility::Interpolator<scalar, scalar>::XY> mjdDut(timePointsMJD.size());

            for (indexType i = 0; i < timePointsMJD.size(); ++i) {
                mjdDut[i] = {timePointsMJD[i], dut[i]};
            }

            interpolator = Utility::Interpolator<scalar, scalar>(mjdDut);
        };


        [[nodiscard]] scalar dut(const Time<TimeScale::UTC_SCALE> &timeUTC) const noexcept {


            return interpolator.interpolateDUT(timeUTC.mjd());
        }
    };
}

#endif //BALLISTICS2023_DUTCORRECTION_H
