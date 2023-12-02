//
// Created by ivankhripunov on 29.11.23.
//

#ifndef BALLISTICS2023_TIME_H
#define BALLISTICS2023_TIME_H

#include "sofa/sofa.h"
#include "utility/types/BasicTypes.h"
#include "exceptions/TimeExceptions.h"

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

//TODO: убрать RealType!

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


    template<typename RealType, TimeScale>
    class Time {
        // количество дней, начиная с полудня 1 января 4713 года до н.э.
        RealType jdDayInt_; // целая часть
        RealType jdDayFrac_; // 0 < дробная часть дня < 1

    public:

        static constexpr RealType JD_MINUS_MJD = static_cast<RealType>(2400000) + static_cast<RealType>(0.5);
        static constexpr RealType SECS_PER_DAY = 86400;

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
        constexpr Time(RealType jdInt, RealType jdFrac) noexcept;

        /**
         * Создает объект Time по данному JD
         * @param jd время в формате JD
         * @return объект Time
         */
        [[nodiscard]] constexpr static Time buildFromJD(RealType jd) noexcept;

        /**
         * Cоздает объект Time по данному MJD
         * @param mjd время в формате MJD
         * @return объект Time
         */
        [[nodiscard]] constexpr static Time buildFromMJD(RealType mjd) noexcept;

        /**
         * Создает объект Time по данному моменту в календарном формате, используя SOFA
         * @param year год
         * @param month месяц
         * @param day день
         * @param hour час
         * @param minute минута
         * @param seconds секунда
         * @return объект Time
         */
        [[nodiscard]] constexpr static Time
        buildFromCalendar(int year, int month, int day, int hour, int minute, RealType seconds);


        /**
         * @return Целое количество дней по шкале JD
         */
        [[nodiscard]] constexpr RealType jdDayInt() const noexcept;

        /**
         * @return Дробное количество дней по шкале JD
         */
        [[nodiscard]] constexpr RealType jdDayFrac() const noexcept;

        /**
         * @return Время по шкале JD
         */
        [[nodiscard]] constexpr RealType jd() const noexcept;

        /**
         * @return Время по шкале MJD
         */
        [[nodiscard]] constexpr RealType mjd() const noexcept;

        /**
         * @return Шкала времени Scale
         */
        [[nodiscard]] constexpr TimeScale scale() const noexcept;

        /**
         * Вычитает из одного момента времени другой
         * @param начальный момент времени
         * @return разница по времени в секундах
         */
        [[nodiscard]] constexpr RealType operator-(const Time &other) const noexcept;

        /**
         * Вычитает из момента времени данное количество секунд
         */
        [[nodiscard]] constexpr Time operator-(RealType seconds) const noexcept;

        /**
         * Прибавляет к моменту времени данное количество секунд
         */
        [[nodiscard]] constexpr Time operator+(RealType seconds) const noexcept;

        /**
         * @return Сравнение положения двух моментов времени на временной шкале JD
         */
        [[nodiscard]] auto operator<=>(const Time &other) const noexcept = default;
    };


    template<typename RealType, TimeScale Scale>
    constexpr Time<RealType, Scale> Time<RealType, Scale>::buildFromJD(RealType jd) noexcept {
        return Time<RealType, Scale>(jd, static_cast<RealType>(0));
    }


    template<typename RealType, TimeScale Scale>
    constexpr void Time<RealType, Scale>::align() {

        // Пример работы:
        // На вход -8.4, 4.5
        // Округляем до -9 и 4.
        // Суммарная разница с исходными будет 1.1
        // Тогда в целую часть запишется -9 + 4 + 1 = -4, в дробную 0.1

        // округляем вниз (если отрицательное то, кроме отбрасывания дробной части, нужно еще вычесть единичку)
        const int jdIntFloor =
                jdDayInt_ >= static_cast<RealType>(0) ? static_cast<int>(jdDayInt_) : static_cast<int>(jdDayInt_) - 1;
        const int jdFracFloor =
                jdDayFrac_ >= static_cast<RealType>(0) ? static_cast<int>(jdDayFrac_) : static_cast<int>(jdDayFrac_) -
                                                                                        1;

        // ищем разницу между округлением и исходным значением. Нет static_cast, т.к. расширяющее преобразование типов безопасно
        const RealType jdIntDelta = jdDayInt_ - static_cast<RealType>(jdIntFloor);
        const RealType jdFracDelta = jdDayFrac_ - static_cast<RealType>(jdFracFloor);

        // ищем суммарную разницу (она неотрицательная и не превышает 2, т.к. округляли в пол)
        const RealType jdDeltasSum = jdIntDelta + jdFracDelta;

        // если разница >= 1 сутки, то целая часть равна сумме округленных + 1
        // если разница < 1 cуток, то прибавлять 1 не надо
        // таким образом, гарантируется, что jdDayFrac_ < 1. Сохраняется инвариант класса
        if (jdDeltasSum >= static_cast<RealType>(1)) {
            jdDayInt_ = static_cast<RealType>(jdIntFloor + jdFracFloor + 1);
            jdDayFrac_ = jdDeltasSum - static_cast<RealType>(1);
        } else {
            jdDayInt_ = static_cast<RealType>(jdIntFloor + jdFracFloor);
            jdDayFrac_ = jdDeltasSum;
        }
    }


    template<typename RealType, TimeScale Scale>
    constexpr
    Time<RealType, Scale>::Time() noexcept : jdDayInt_(static_cast<RealType>(0)),
                                             jdDayFrac_(static_cast<RealType>(0)) {}


    template<typename RealType, TimeScale Scale>
    constexpr Time<RealType, Scale>::Time(const RealType jdInt, const RealType jdFrac) noexcept : jdDayInt_(jdInt),
                                                                                                  jdDayFrac_(jdFrac) {
        // Входные данные могут быть любыми, но мы выполняем приведение к нужному виду
        align();
    }


    template<typename RealType, TimeScale Scale>
    constexpr Time<RealType, Scale> Time<RealType, Scale>::buildFromMJD(RealType mjd) noexcept {
        return Time<RealType, Scale>(mjd, JD_MINUS_MJD);
    }


    template<typename RealType, TimeScale Scale>
    constexpr Time<RealType, Scale>
    Time<RealType, Scale>::buildFromCalendar(int year, int month, int day, int hour, int minute, RealType seconds) {

        //потому что SOFA принимает double
        double jdDay;
        double jdDayPart;

        const int status = static_cast<int>(
                iauDtf2d(static_cast<std::string>(SCALES[static_cast<indexType>(Scale)]).c_str(),
                         year, month, day, hour, minute, seconds, &jdDay, &jdDayPart));

        if (!(status == 0 || status == 1)) {
            // Sofa возвращает:
            // 0, если все в порядке
            // 1, если дата слишком дальняя
            // -1, -2, -3, -4, -5, если все плохо c годами, месяцами, днями, минутами, секундами соответственно
            throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED FABRIC FROM CALENDAR");
        }

        return Time<RealType, Scale>(static_cast<RealType>(jdDay), static_cast<RealType>(jdDayPart));

    }


    template<typename RealType, TimeScale Scale>
    constexpr RealType Time<RealType, Scale>::jdDayInt() const noexcept {
        return jdDayInt_;
    }


    template<typename RealType, TimeScale Scale>
    constexpr RealType Time<RealType, Scale>::jdDayFrac() const noexcept {
        return jdDayFrac_;
    }


    template<typename RealType, TimeScale Scale>
    constexpr RealType Time<RealType, Scale>::jd() const noexcept {
        return jdDayInt_ + jdDayFrac_;
    }

    template<typename RealType, TimeScale Scale>
    constexpr RealType Time<RealType, Scale>::mjd() const noexcept {
        //такой порядок важен, чтобы сохранить точность
        return (jdDayInt_ - JD_MINUS_MJD) + jdDayFrac_;
    }


    template<typename RealType, TimeScale Scale>
    constexpr TimeScale Time<RealType, Scale>::scale() const noexcept {
        return Scale;
    }


    template<typename RealType, TimeScale Scale>
    constexpr RealType Time<RealType, Scale>::operator-(const Time &other) const noexcept {

        const RealType jdIntDelta = jdDayInt_ - other.jdDayInt_;
        const RealType jdFracDelta = jdDayFrac_ - other.jdDayFrac_;
        return (jdIntDelta + jdFracDelta) * SECS_PER_DAY;
    }


    template<typename RealType, TimeScale Scale>
    constexpr Time<RealType, Scale> Time<RealType, Scale>::operator-(const RealType seconds) const noexcept {
        return Time<RealType, Scale>(jdDayInt_, jdDayFrac_ - seconds / SECS_PER_DAY);
    }


    template<typename RealType, TimeScale Scale>
    constexpr Time<RealType, Scale> Time<RealType, Scale>::operator+(const RealType seconds) const noexcept {
        return Time<RealType, Scale>(jdDayInt_, jdDayFrac_ + seconds / SECS_PER_DAY);
    }
}

#endif //BALLISTICS2023_TIME_H
