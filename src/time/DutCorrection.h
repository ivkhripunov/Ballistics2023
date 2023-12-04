//
// Created by ivankhripunov on 30.11.23.
//

#ifndef BALLISTICS2023_DUTCORRECTION_H
#define BALLISTICS2023_DUTCORRECTION_H

#include "exceptions/TimeExceptions.h"
#include "Time.h"

namespace Ballistics::TimeModule {

    template<typename xType, typename yType>
    class Interpolator {
    private:
        Containers::vector<xType> x_;
        Containers::vector<yType> y_;
        Containers::vector<xType> frac_;

    public:
        explicit Interpolator(const Containers::vector<xType> &xArray, const Containers::vector<yType> &yArray) {

            const indexType size = xArray.size();
            x_.resize(size);
            y_.resize(size);
            frac_.resize(size - 1);

            for (indexType i = 0; i < size - 1; ++i) {
                x_[i] = xArray[i];
                y_[i] = yArray[i];
                frac_[i] = (yArray[i + 1] - yArray[i]) / (xArray[i + 1] - xArray[i]);
            }
            x_[size - 1] = xArray[size - 1];
            y_[size - 1] = yArray[size - 1];
        };

        /**
         * Возвращает значение линейного интерполянта в точке
         */
        [[nodiscard]] scalar interpolate(const scalar xPoint) const {

            for (indexType i = 0; i < x_.size() - 1; ++i) {

                if ((xPoint >= x_[i]) && (xPoint < x_[i + 1])) {

                    return y_[i] + frac_[i] * (xPoint - x_[i]);
                }
            }

            if (xPoint == x_[x_.size() - 1]) {
                return y_[y_.size() - 1];
            }

            throw Ballistics::Exceptions::TimeModuleException("INTERPOLATOR ERROR: VALUE OUT OF BOUNDS");

        }

    };

    class DutContainer {

    private:
        Interpolator<scalar, scalar> interpolator;

    public:

        DutContainer(const Containers::vector<scalar> &timePointsMJD, const Containers::vector<scalar> &dut) noexcept
                : interpolator(timePointsMJD, dut) {};


        [[nodiscard]] scalar dut(const Time<TimeScale::UTC_SCALE> &timeUTC) const noexcept {

            return interpolator.interpolate(timeUTC.mjd());
        }
    };
}

#endif //BALLISTICS2023_DUTCORRECTION_H
