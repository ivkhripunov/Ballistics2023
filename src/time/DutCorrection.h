//
// Created by ivankhripunov on 30.11.23.
//

#ifndef BALLISTICS2023_DUTCORRECTION_H
#define BALLISTICS2023_DUTCORRECTION_H

#include "Time.h"
#include "utility/interpolator/LinearInterpolator.h"

//TODO: add leap seconds control

namespace Ballistics::TimeModule {

    class DutContainer {

    private:
        Utility::Interpolator<scalar, scalar> interpolator;

    public:

        DutContainer(const Containers::vector<scalar> &timePointsMJD, const Containers::vector<scalar> &dut) noexcept
                : interpolator(timePointsMJD, dut) {};


        [[nodiscard]] scalar dut(const Time<TimeScale::UTC_SCALE> &timeUTC) const noexcept {

            return interpolator.interpolate(timeUTC.mjd());
        }
    };
}

#endif //BALLISTICS2023_DUTCORRECTION_H
