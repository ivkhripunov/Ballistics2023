//
// Created by ivankhripunov on 29.11.23.
//

#ifndef BALLISTICS2023_TIME_H
#define BALLISTICS2023_TIME_H

#include "../Types/BasicTypes.h"

//Вопросы:
//1. Зачем static
//2. Почему не храним jdInt_ в int
//3. Нужен ли explicit в конструкторе

namespace Ballistics::TimeModule {

    enum class TimeScale {
        UT1_SCALE = 0,
        UTC_SCALE = 1,
        TAI_SCALE = 2,
        TT_SCALE  = 3,
        TCG_SCALE = 4,
        TCB_SCALE = 5,
        TDB_SCALE = 6,
    };


template<typename RealType, TimeScale>
    class Time {
        // количество дней, начиная с полудня 1 января 4713 года до н.э.
        RealType jdDayInt_; // целая часть
        RealType jdDayFrac_; // 0 < дробная часть дня < 1

    public:
        // Необходимо отдельно обрабатывать целую и дробную часть в конструкторе, иначе проблемы с FPA.
        // В противном случае, целая часть при сложении может съесть дробную или наоборот.
        Time(const RealType jdInt, const RealType jdFrac) noexcept {

            // Пример работы:
            // На вход -8.4, 4.5
            // Округляем до -9 и 4.
            // Суммарная разница с исходными будет 1.1
            // Тогда в целую часть запишется -9 + 4 + 1 = -4, в дробную 0.1

            // округляем вниз (если отрицательное то, кроме отбрасывания дробной части, нужно еще вычесть единичку)
            const int jdIntFloor =
                    jdInt >= static_cast<RealType>(0) ? static_cast<int>(jdInt) : static_cast<int>(jdInt) - 1;
            const int jdFracFloor =
                    jdFrac >= static_cast<RealType>(0) ? static_cast<int>(jdFrac) : static_cast<int>(jdFrac) - 1;

            // ищем разницу между округлением и исходным значением. Нет static_cast, т.к. расширяющее преобразование типов безопасно
            const RealType jdIntDelta = jdInt - static_cast<RealType>(jdIntFloor);
            const RealType jdFracDelta = jdFrac - static_cast<RealType>(jdFracFloor);

            // ищем суммарную разницу (она неотрицательная и не превышает 2, т.к. округляли в пол)
            const RealType jdDeltasSum = jdIntDelta + jdFracDelta;

            // если разница >= 1 сутки, то целая часть равна сумме округленных + 1
            // если разница < 1 cуток, то прибавлять 1 не надо
            // таким образом, гарантируется, что jdDayFrac_ < 1.
            if (jdDeltasSum >= static_cast<RealType>(1)) {
                jdDayInt_ = static_cast<RealType>(jdIntFloor + jdFracFloor + 1);
                jdDayFrac_ = jdDeltasSum - static_cast<RealType>(1);
            } else {
                jdDayInt_ = static_cast<RealType>(jdIntFloor + jdFracFloor);
                jdDayFrac_ = jdDeltasSum;
            }
        }

//        Time static fromJD(const RealType jd) noexcept {}

    };

}

#endif //BALLISTICS2023_TIME_H
