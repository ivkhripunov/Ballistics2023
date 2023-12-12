//
// Created by ivankhripunov on 09.12.23.
//

#ifndef BALLISTICS2023_LINEARINTERPOLATOR_H
#define BALLISTICS2023_LINEARINTERPOLATOR_H

//TODO: optimize, add reference field, fix exception

#include "utility/types/BasicTypes.h"
#include "exceptions/TimeExceptions.h"

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

        Interpolator(const Containers::vector<XY> xy) noexcept : data_(xy) {};

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

            throw Exceptions::TimeModuleException("INTERPOLATOR ERROR: VALUE OUT OF BOUNDS");

        }

    };

}

#endif //BALLISTICS2023_LINEARINTERPOLATOR_H
