//
// Created by ivankhripunov on 08.12.23.
//

#ifndef BALLISTICS2023_POLARMOTIONCONTAINER_H
#define BALLISTICS2023_POLARMOTIONCONTAINER_H

#include "time/TimeConverter.h"

namespace Ballistics::FrameModule {

    class PolarMotionContainer {

        struct PolarMotion {
            scalar xp_;
            scalar yp_;
        };

        //Времена
        Containers::vector<TimeModule::Time<TimeModule::TimeScale::UTC_SCALE>> timeData_;
        //Параметры полярного движения
        Containers::vector<PolarMotion> polarMotionData_;

    public:

        PolarMotionContainer(const Containers::vector<TimeModule::Time<TimeModule::TimeScale::UTC_SCALE>> &data,
                             const Containers::vector<scalar> &xp, const Containers::vector<scalar> &yp) noexcept
                : timeData_(data) {

            indexType size = timeData_.size();
            polarMotionData_.resize(size);

            for (indexType i = 0; i < size; ++i) {
                polarMotionData_[i] = {xp[i], yp[i]};
            }
        }

        /**
         * Интерполяция по массивам
         * @param utc точка интерполяции
         * @return значение интерполированного PolarMotion в данной точке
         */
        [[nodiscard]] PolarMotion
        getPolarMotion(const TimeModule::Time<TimeModule::TimeScale::UTC_SCALE> &utc) const noexcept;
    };
}


#endif //BALLISTICS2023_POLARMOTIONCONTAINER_H
