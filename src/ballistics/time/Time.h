//
// Created by ivankhripunov on 29.11.23.
//

#ifndef BALLISTICS2023_TIME_H
#define BALLISTICS2023_TIME_H

#include "sofa/sofa.h"
#include "ballistics/utility/types/BasicTypes.h"
#include "ballistics/exceptions/Exceptions.h"

//Вопросы:
//1. Зачем static
//2. Почему не храним jdInt_ в int
//3. Нужен ли explicit в конструкторе
//4. Насколько тут нужны static_cast
//5. Почему на satellite_system не реализован spaceship
//6. Зачем нужны фабрики? Почему не сделать отдельную от класса функцию
//7. Подробнее, что такое consteval и почему мы его здесь не используем
//8. Почему в sat_system JD_MINUS_MJD, если MJD отсчитывается от 00:00:00 17.11.1858
//9. Изменит что либо добавление inline к методами?
//10. Почему для оператора - в случае Time - Time важен const, а в случае Time - seconds не нужен

//Ответы
//1. Это фабрика конструкторов
//2. Можно и так, просто будет больше возни с конструкторами
//3. Explicit запрещает неявное преобразование типов, т.е. запрещает ситуацию, когда A(double), B(A) и работает B(double)
//4.
//5. Потому что сат систем должен компилироваться на микроконтроллерах, а там спейсшип не поддерживается
//6. Фабрики нужны, потому что mjd и jd  одиакового типа, но конструкторы для них должны быть разные
//7. Consteval требует, чтобы ОБЯЗАТЕЛЬНО вычислялось на этапе компиляции, constexpr подсказывает компилятору, что это МОЖЕТ БЫТЬ сделано
//8. Логично, ведь тогда разница будет положительной :-)
//9.
//10.

//Заметки:
// Сначала сделал приведение по целой и дробной части внутри конструктора, но потом вынес в отдельную функцию. Так удобнее реализовывать
// Необходимо отдельно обрабатывать целую и дробную часть, иначе проблемы с FPA.
// В противном случае целая часть при сложении может съесть дробную или наоборот.
// Конструктор инвариантен относительно перестановки входных данных jd_int и  jd_frac
// В buildFromCalendar нет noexception, т.к. софа может выкидывать исключения
// про iauD2dtf можно почитать в кукбуке SOFA
// Не надо путать шкалы и форматы времени. Календарь и JD это форматы (различные формы записи). Формат определяет начало отсчета
// Шкала отвечает за скорость изменения времени

//TODO: убрать scalar!

namespace Ballistics::TimeModule {

    enum class TimeScale {
        UT1_SCALE = 0,
        UTC_SCALE = 1,
        TAI_SCALE = 2,
        TT_SCALE = 3,
        TCG_SCALE = 4,
        TCB_SCALE = 5,
        TDB_SCALE = 6,
    };

    //Нужно для инпута в софу
    inline constexpr std::string_view UT1 = "ut1";
    inline constexpr std::string_view UTC = "UTC";
    inline constexpr std::string_view TAI = "TAI";
    inline constexpr std::string_view TT = "tt";
    inline constexpr std::string_view TCG = "tcg";
    inline constexpr std::string_view TCB = "tcb";
    inline constexpr std::string_view TDB = "tdb";
    inline constexpr std::array<std::string_view, 7> SCALES = {UT1, UTC, TAI, TT, TCG, TCB, TDB};


    template<TimeScale>
    class Time {
        // количество дней, начиная с полудня 1 января 4713 года до н.э.
        scalar jdDayInt_; // целая часть
        scalar jdDayFrac_; // 0 < дробная часть дня < 1

    public:

        static constexpr scalar JD_MINUS_MJD = static_cast<scalar>(2400000) + static_cast<scalar>(0.5);
        static constexpr scalar SECS_PER_DAY = 86400;

        /** Приводим целую и дробную часть к правильному виду **/
        constexpr void align();

        /**
         * Стандартный конструктор, инициализация нулевым моментом времени по JD
         */
        constexpr Time() noexcept;

        /**
         * Конструктор по времени в формате JD
         * @param jdInt целая часть
         * @param jdFrac дробная часть
         */
        constexpr Time(scalar jdInt, scalar jdFrac) noexcept;

        /**
         * Создает объект Time по данному JD
         * @param jd время в формате JD
         * @return объект Time
         */
        [[nodiscard]] constexpr static Time buildFromJD(scalar jd) noexcept;

        /**
         * Cоздает объект Time по данному MJD
         * @param mjd время в формате MJD
         * @return объект Time
         */
        [[nodiscard]] constexpr static Time buildFromMJD(scalar mjd) noexcept;

        /**
         * @return Целое количество дней по шкале JD
         */
        [[nodiscard]] constexpr scalar jdDayInt() const noexcept;

        /**
         * @return Дробное количество дней по шкале JD
         */
        [[nodiscard]] constexpr scalar jdDayFrac() const noexcept;

        /**
         * @return Время по шкале JD
         */
        [[nodiscard]] constexpr scalar jd() const noexcept;

        /**
         * @return Время по шкале MJD
         */
        [[nodiscard]] constexpr scalar mjd() const noexcept;

        /**
         * @return Шкала времени Scale
         */
        [[nodiscard]] constexpr TimeScale scale() const noexcept;

        /**
         * Вычитает из одного момента времени другой
         * @param начальный момент времени
         * @return разница по времени в секундах
         */
        [[nodiscard]] constexpr scalar operator-(const Time &other) const noexcept;

        /**
         * Вычитает из момента времени данное количество секунд
         */
        [[nodiscard]] constexpr Time operator-(scalar seconds) const noexcept;

        /**
         * Прибавляет к моменту времени данное количество секунд
         */
        [[nodiscard]] constexpr Time operator+(scalar seconds) const noexcept;

        /**
         * @return Сравнение положения двух моментов времени на временной шкале JD
         */
        [[nodiscard]] auto operator<=>(const Time &other) const noexcept = default;
    };


    template<TimeScale Scale>
    constexpr Time<Scale> Time<Scale>::buildFromJD(scalar jd) noexcept {
        return Time<Scale>(jd, static_cast<scalar>(0));
    }


    template<TimeScale Scale>
    constexpr void Time<Scale>::align() {

        // Пример работы:
        // На вход -8.4, 4.5
        // Округляем до -9 и 4.
        // Суммарная разница с исходными будет 1.1
        // Тогда в целую часть запишется -9 + 4 + 1 = -4, в дробную 0.1

        // округляем вниз (если отрицательное то, кроме отбрасывания дробной части, нужно еще вычесть единичку)
        const int jdIntFloor =
                jdDayInt_ >= static_cast<scalar>(0) ? static_cast<int>(jdDayInt_) : static_cast<int>(jdDayInt_) - 1;
        const int jdFracFloor =
                jdDayFrac_ >= static_cast<scalar>(0) ? static_cast<int>(jdDayFrac_) : static_cast<int>(jdDayFrac_) -
                                                                                      1;

        // ищем разницу между округлением и исходным значением. Нет static_cast, т.к. расширяющее преобразование типов безопасно
        const scalar jdIntDelta = jdDayInt_ - static_cast<scalar>(jdIntFloor);
        const scalar jdFracDelta = jdDayFrac_ - static_cast<scalar>(jdFracFloor);

        // ищем суммарную разницу (она неотрицательная и не превышает 2, т.к. округляли в пол)
        const scalar jdDeltasSum = jdIntDelta + jdFracDelta;

        // если разница >= 1 сутки, то целая часть равна сумме округленных + 1
        // если разница < 1 cуток, то прибавлять 1 не надо
        // таким образом, гарантируется, что jdDayFrac_ < 1. Сохраняется инвариант класса
        if (jdDeltasSum >= static_cast<scalar>(1)) {
            jdDayInt_ = static_cast<scalar>(jdIntFloor + jdFracFloor + 1);
            jdDayFrac_ = jdDeltasSum - static_cast<scalar>(1);
        } else {
            jdDayInt_ = static_cast<scalar>(jdIntFloor + jdFracFloor);
            jdDayFrac_ = jdDeltasSum;
        }
    }


    template<TimeScale Scale>
    constexpr
    Time<Scale>::Time() noexcept : jdDayInt_(static_cast<scalar>(0)),
                                   jdDayFrac_(static_cast<scalar>(0)) {}


    template<TimeScale Scale>
    constexpr Time<Scale>::Time(const scalar jdInt, const scalar jdFrac) noexcept : jdDayInt_(jdInt),
                                                                                    jdDayFrac_(jdFrac) {
        // Входные данные могут быть любыми, но мы выполняем приведение к нужному виду
        align();
    }


    template<TimeScale Scale>
    constexpr Time<Scale> Time<Scale>::buildFromMJD(scalar mjd) noexcept {
        return Time<Scale>(mjd, JD_MINUS_MJD);
    }


    template<TimeScale Scale>
    constexpr scalar Time<Scale>::jdDayInt() const noexcept {
        return jdDayInt_;
    }


    template<TimeScale Scale>
    constexpr scalar Time<Scale>::jdDayFrac() const noexcept {
        return jdDayFrac_;
    }


    template<TimeScale Scale>
    constexpr scalar Time<Scale>::jd() const noexcept {
        return jdDayInt_ + jdDayFrac_;
    }

    template<TimeScale Scale>
    constexpr scalar Time<Scale>::mjd() const noexcept {
        //такой порядок важен, чтобы сохранить точность
        return (jdDayInt_ - JD_MINUS_MJD) + jdDayFrac_;
    }


    template<TimeScale Scale>
    constexpr TimeScale Time<Scale>::scale() const noexcept {
        return Scale;
    }


    template<TimeScale Scale>
    constexpr scalar Time<Scale>::operator-(const Time &other) const noexcept {

        const scalar jdIntDelta = jdDayInt_ - other.jdDayInt_;
        const scalar jdFracDelta = jdDayFrac_ - other.jdDayFrac_;
        return (jdIntDelta + jdFracDelta) * SECS_PER_DAY;
    }


    template<TimeScale Scale>
    constexpr Time<Scale> Time<Scale>::operator-(const scalar seconds) const noexcept {
        return Time<Scale>(jdDayInt_, jdDayFrac_ - seconds / SECS_PER_DAY);
    }


    template<TimeScale Scale>
    constexpr Time<Scale> Time<Scale>::operator+(const scalar seconds) const noexcept {
        return Time<Scale>(jdDayInt_, jdDayFrac_ + seconds / SECS_PER_DAY);
    }
}

#endif //BALLISTICS2023_TIME_H
