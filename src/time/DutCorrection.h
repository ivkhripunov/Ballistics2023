//
// Created by ivankhripunov on 30.11.23.
//

#ifndef BALLISTICS2023_DUTCORRECTION_H
#define BALLISTICS2023_DUTCORRECTION_H

#include "../types/BasicTypes.h"
#include "exceptions/TimeExceptions.h"
#include "Time.h"

//1. Если подают момент за пределами интерполяции, выкидывать исключение?

namespace Ballistics::TimeModule {

    template<typename xType, typename yType>
    struct Pair {
        xType x;
        yType y;
    };

    template<typename xType, typename yType>
    class Interpolator {
    private:
        Containers::vector<Pair<xType, yType>> data_;

    public:
        explicit Interpolator(const Containers::vector<xType> &xArray, const Containers::vector<yType> &yArray) {

            data_.resize(xArray.size());

            for (indexType i = 0; i < xArray.size(); ++i) {
                data_[i] = {xArray[i], yArray[i]};
            }
        };

        /**
         * Возвращает значение линейного интерполянта в точке
         */
        [[nodiscard]] scalar interpolate(const scalar xPoint) const {

            for (indexType i = 0; i < data_.size() - 1; ++i) {

                if ((xPoint >= data_[i].x) && (xPoint < data_[i + 1].x)) {
                    const yType numerator = data_[i + 1].y - data_[i].y;
                    const xType denominator = data_[i + 1].x - data_[i].x;

                    return data_[i].y + numerator / denominator * (xPoint - data_[i].x);
                }
            }

            if (xPoint == data_[data_.size() - 1].x) {
                return data_[data_.size() - 1].y;
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


        [[nodiscard]] scalar dut(const Time<scalar, TimeScale::UTC_SCALE> &timeUTC) const noexcept {

            return interpolator.interpolate(timeUTC.mjd());
        }
    };
}

#endif //BALLISTICS2023_DUTCORRECTION_H
