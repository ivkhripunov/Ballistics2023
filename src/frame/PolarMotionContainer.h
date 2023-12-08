//
// Created by ivankhripunov on 08.12.23.
//

#ifndef BALLISTICS2023_POLARMOTIONCONTAINER_H
#define BALLISTICS2023_POLARMOTIONCONTAINER_H

#include "utility/types/BasicTypes.h"
#include "time/Time.h"

namespace Ballistics::Frame {

    class PolarMotionContainer {

        struct PolarMotion {
            scalar xp_;
            scalar yp_;
        };

        //Дата
        Containers::vector<TimeModule::Time<TimeModule::TimeScale::UTC_SCALE>> data_;
        //Параметры полярного движения
        Containers::vector<scalar> xp_;
        Containers::vector<scalar> yp_;

        /**
         * Интерполяция по массивам
         * @param utc точка интерполяции
         * @return значение PolarMotion в данной точке
         */
        [[nodiscard]] PolarMotion
        getPolarMotion(const TimeModule::Time<TimeModule::TimeScale::UTC_SCALE> &utc) const noexcept;
    };

}

#endif //BALLISTICS2023_POLARMOTIONCONTAINER_H
