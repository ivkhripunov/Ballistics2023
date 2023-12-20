//
// Created by ivankhripunov on 08.12.23.
//

#ifndef BALLISTICS2023_POLARMOTIONCONTAINER_H
#define BALLISTICS2023_POLARMOTIONCONTAINER_H

#include "ballistics/time/TimeConverter.h"

//TODO: add interpolator

namespace Ballistics::FrameModule {


    struct PolarMotion {
        scalar xp_;
        scalar yp_;
    };

    class PolarMotionContainer {

        //Времена
        Containers::vector<scalar> timeData_; //UTC MJD
        //Параметры полярного движения
        Containers::vector<PolarMotion> polarMotionData_;

    public:

        PolarMotionContainer(const Containers::vector<scalar> &timeData,
                             const Containers::vector<scalar> &xp, const Containers::vector<scalar> &yp) noexcept
                : timeData_(timeData) {

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

    PolarMotion
    PolarMotionContainer::getPolarMotion(const TimeModule::Time<TimeModule::TimeScale::UTC_SCALE> &utc) const noexcept {

        const scalar utcMjd = utc.mjd();
        for (indexType i = 0; i < timeData_.size() - 1; ++i) {

            if ((utcMjd >= timeData_[i]) && (utcMjd < timeData_[i + 1])) {
                const scalar numeratorY = polarMotionData_[i + 1].yp_ - polarMotionData_[i].yp_;
                const scalar numeratorX = polarMotionData_[i + 1].xp_ - polarMotionData_[i].xp_;

                const scalar denominator = timeData_[i + 1] - timeData_[i];
                const scalar timeDelta = utcMjd - timeData_[i];

                const scalar yp = polarMotionData_[i].yp_ + numeratorY / denominator * timeDelta;
                const scalar xp = polarMotionData_[i].xp_ + numeratorX / denominator * timeDelta;

                return {xp, yp};
            }
        }

        if (utcMjd == timeData_[timeData_.size() - 1]) {
            return {polarMotionData_[polarMotionData_.size() - 1].xp_,
                    polarMotionData_[polarMotionData_.size() - 1].yp_};
        }

        throw Exceptions::Exception("POLAR MOTION INTERPOLATOR ERROR: VALUE OUT OF BOUNDS");
    }
}


#endif //BALLISTICS2023_POLARMOTIONCONTAINER_H
