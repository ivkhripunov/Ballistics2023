//
// Created by ivankhripunov on 09.12.23.
//

#ifndef BALLISTICS2023_LINEARINTERPOLATOR_H
#define BALLISTICS2023_LINEARINTERPOLATOR_H

#include "ballistics/utility/types/BasicTypes.h"
#include "ballistics/exceptions/Exceptions.h"

namespace Ballistics::Utility {

    template<typename xType, typename yType>
    class Interpolator {

    public:

        struct XY {
            xType x;
            yType y;
        };

    private:
        Containers::vector<XY> data_;

    public:
        Interpolator() = default;

        Interpolator(const Containers::vector<XY> &xy) noexcept: data_(xy) {};

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

            throw Exceptions::Exception("INTERPOLATOR ERROR: VALUE OUT OF BOUNDS");

        }

        [[nodiscard]] scalar interpolateDUT(const scalar xPoint) const {

            for (indexType i = 0; i < data_.size() - 1; ++i) {

                if ((xPoint >= data_[i].x) && (xPoint < data_[i + 1].x)) {

                    const yType dutDifference = data_[i + 1].y - data_[i].y;
                    const yType leapAdd = dutDifference > static_cast<yType>(0.6) ?
                                          -static_cast<yType>(1) :
                                          dutDifference < static_cast<yType>(-0.6) ?
                                          static_cast<yType>(1) : static_cast<yType>(0);

                    const yType numerator = data_[i + 1].y - data_[i].y + leapAdd;
                    const xType denominator = data_[i + 1].x - data_[i].x;

                    return data_[i].y + numerator / denominator * (xPoint - data_[i].x);
                }
            }

            if (xPoint == data_[data_.size() - 1].x) {
                return data_[data_.size() - 1].y;
            }

            throw Exceptions::Exception("INTERPOLATOR ERROR: VALUE OUT OF BOUNDS");

        }

    };

}

#endif //BALLISTICS2023_LINEARINTERPOLATOR_H
