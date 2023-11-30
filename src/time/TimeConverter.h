//
// Created by ivankhripunov on 30.11.23.
//

#ifndef BALLISTICS2023_TIMECONVERTER_H
#define BALLISTICS2023_TIMECONVERTER_H

#include "Time.h"

//1. NRVO? в конвертации

//Заметки
//При реализации конвертации через софу реализуем только переход на соседнюю шкалу. Все остальное используем уже "готовое"

namespace Ballistics::TimeModule {

    using TimeScale = Ballistics::TimeModule::TimeScale;


    template<typename RealType, typename DutContainer>
    class TimeConverter {
        DutContainer dutContainer_;

        //это финт ушами, чтобы "обмануть компилятор. В макросе компилятор сделает статик ассерт в самом начале,
        //несмотря на else. Это приведет к тому, что, если подать туда false, то программа будет всегда падать на
        //этапе компиляции. С таким финтом мы можем гарантировать, что программа упадет только если дойдет до этого else
        template<TimeScale Scale>
        static constexpr bool AlwaysFalse = false;

        static constexpr scalar deltaTT_TAI = static_cast<scalar>(32.183);
    public:
        /**
         * Cоздание конвертера через дут контейнер
         */
        explicit TimeConverter(const DutContainer &dutContainer) noexcept;

        /**
         * Функция для конвертации между шкалами времени
         * @tparam To Целевая шкала времени
         * @tparam From Исходная шкала времени
         * @param from Объект времени в исходной шкале
         * @return Объект времени в целевой шкале
         */
        template<TimeScale To, TimeScale From>
        Time<RealType, To> convert(const Time<RealType, From> &from) const;

        /**************************************************************************************************************\
                                                    UT1 в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<RealType, TimeScale::UTC_SCALE>
        convertUT1_UTC(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const;

        [[nodiscard]] Time<RealType, TimeScale::TAI_SCALE>
        convertUT1_TAI(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const;

        [[nodiscard]] Time<RealType, TimeScale::TT_SCALE>
        convertUT1_TT(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const;

        [[nodiscard]] Time<RealType, TimeScale::TCG_SCALE>
        convertUT1_TCG(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const;

        [[nodiscard]] Time<RealType, TimeScale::TDB_SCALE>
        convertUT1_TDB(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const;

        [[nodiscard]] Time<RealType, TimeScale::TCB_SCALE>
        convertUT1_TCB(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const;

        /**************************************************************************************************************\
                                                    UTC в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<RealType, TimeScale::TAI_SCALE>
        convertUTC_TAI(const Time<RealType, TimeScale::UTC_SCALE> &utc) const;

        [[nodiscard]] Time<RealType, TimeScale::UT1_SCALE>
        convertUTC_UT1(const Time<RealType, TimeScale::UTC_SCALE> &utc) const;

        [[nodiscard]] Time<RealType, TimeScale::TT_SCALE>
        convertUTC_TT(const Time<RealType, TimeScale::UTC_SCALE> &utc) const;

        [[nodiscard]] Time<RealType, TimeScale::TCG_SCALE>
        convertUTC_TCG(const Time<RealType, TimeScale::UTC_SCALE> &utc) const;

        [[nodiscard]] Time<RealType, TimeScale::TCB_SCALE>
        convertUTC_TCB(const Time<RealType, TimeScale::UTC_SCALE> &utc) const;

        [[nodiscard]] Time<RealType, TimeScale::TDB_SCALE>
        convertUTC_TDB(const Time<RealType, TimeScale::UTC_SCALE> &utc) const;

        /**************************************************************************************************************\
                                                    TAI в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<RealType, TimeScale::UTC_SCALE>
        convertTAI_UTC(const Time<RealType, TimeScale::TAI_SCALE> &tai) const;

        [[nodiscard]] Time<RealType, TimeScale::UT1_SCALE>
        convertTAI_UT1(const Time<RealType, TimeScale::TAI_SCALE> &tai) const;

        [[nodiscard]] Time<RealType, TimeScale::TT_SCALE>
        convertTAI_TT(const Time<RealType, TimeScale::TAI_SCALE> &tai) const;

        [[nodiscard]] Time<RealType, TimeScale::TCG_SCALE>
        convertTAI_TCG(const Time<RealType, TimeScale::TAI_SCALE> &tai) const;

        [[nodiscard]] Time<RealType, TimeScale::TCB_SCALE>
        convertTAI_TCB(const Time<RealType, TimeScale::TAI_SCALE> &tai) const;

        [[nodiscard]] Time<RealType, TimeScale::TDB_SCALE>
        convertTAI_TDB(const Time<RealType, TimeScale::TAI_SCALE> &tai) const;

        /**************************************************************************************************************\
                                                    TT в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<RealType, TimeScale::UTC_SCALE>
        convertTT_UTC(const Time<RealType, TimeScale::TT_SCALE> &tt) const;

        [[nodiscard]] Time<RealType, TimeScale::UT1_SCALE>
        convertTT_UT1(const Time<RealType, TimeScale::TT_SCALE> &tt) const;

        [[nodiscard]] Time<RealType, TimeScale::TAI_SCALE>
        convertTT_TAI(const Time<RealType, TimeScale::TT_SCALE> &tt) const;

        [[nodiscard]] Time<RealType, TimeScale::TCG_SCALE>
        convertTT_TCG(const Time<RealType, TimeScale::TT_SCALE> &tt) const;

        [[nodiscard]] Time<RealType, TimeScale::TCB_SCALE>
        convertTT_TCB(const Time<RealType, TimeScale::TT_SCALE> &tt) const;

        [[nodiscard]] Time<RealType, TimeScale::TDB_SCALE>
        convertTT_TDB(const Time<RealType, TimeScale::TT_SCALE> &tt) const;

        /**************************************************************************************************************\
                                                    TCG в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<RealType, TimeScale::UTC_SCALE>
        convertTCG_UTC(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const;

        [[nodiscard]] Time<RealType, TimeScale::UT1_SCALE>
        convertTCG_UT1(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const;

        [[nodiscard]] Time<RealType, TimeScale::TAI_SCALE>
        convertTCG_TAI(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const;

        [[nodiscard]] Time<RealType, TimeScale::TT_SCALE>
        convertTCG_TT(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const;

        [[nodiscard]] Time<RealType, TimeScale::TCB_SCALE>
        convertTCG_TCB(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const;

        [[nodiscard]] Time<RealType, TimeScale::TDB_SCALE>
        convertTCG_TDB(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const;

        /**************************************************************************************************************\
                                                    TCB в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<RealType, TimeScale::UTC_SCALE>
        convertTCB_UTC(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const;

        [[nodiscard]] Time<RealType, TimeScale::UT1_SCALE>
        convertTCB_UT1(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const;

        [[nodiscard]] Time<RealType, TimeScale::TAI_SCALE>
        convertTCB_TAI(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const;

        [[nodiscard]] Time<RealType, TimeScale::TT_SCALE>
        convertTCB_TT(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const;

        [[nodiscard]] Time<RealType, TimeScale::TCG_SCALE>
        convertTCB_TCG(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const;

        [[nodiscard]] Time<RealType, TimeScale::TDB_SCALE>
        convertTCB_TDB(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const;

        /**************************************************************************************************************\
                                                    TDB в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<RealType, TimeScale::UTC_SCALE>
        convertTDB_UTC(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const;

        [[nodiscard]] Time<RealType, TimeScale::UT1_SCALE>
        convertTDB_UT1(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const;

        [[nodiscard]] Time<RealType, TimeScale::TAI_SCALE>
        convertTDB_TAI(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const;

        [[nodiscard]] Time<RealType, TimeScale::TT_SCALE>
        convertTDB_TT(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const;

        [[nodiscard]] Time<RealType, TimeScale::TCG_SCALE>
        convertTDB_TCG(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const;

        [[nodiscard]] Time<RealType, TimeScale::TCB_SCALE>
        convertTDB_TCB(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const;

    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    template<typename RealType, typename DutContainer>
    TimeConverter<RealType, DutContainer>::TimeConverter(const DutContainer &dutContainer) noexcept : dutContainer_(
            dutContainer) {}

    /**************************************************************************************************************\
                                                    UT1 в другие шкалы
    \**************************************************************************************************************/
//TODO: здесь МПИ, не протестировано!!!
    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UTC_SCALE>
    TimeConverter<RealType, DutContainer>::convertUT1_UTC(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const {

        Time<RealType, TimeScale::UTC_SCALE> utc;
        utc.buildFromMJD(ut1.mjd());

        for (int i = 0; i < 3; ++i) {
            utc = ut1 - dutContainer_.dut(utc);
        }

        return utc;
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TAI_SCALE>
    TimeConverter<RealType, DutContainer>::convertUT1_TAI(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const {

        const Time<RealType, TimeScale::UTC_SCALE> utc = convertUT1_UTC(ut1);

        return convertUTC_TAI(utc);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TT_SCALE>
    TimeConverter<RealType, DutContainer>::convertUT1_TT(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const {

        const Time<RealType, TimeScale::TAI_SCALE> tai = convertUT1_TAI(ut1);

        return convertTAI_TT(tai);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCG_SCALE>
    TimeConverter<RealType, DutContainer>::convertUT1_TCG(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertUT1_TT(ut1);

        return convertTT_TCG(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TDB_SCALE>
    TimeConverter<RealType, DutContainer>::convertUT1_TDB(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertUT1_TT(ut1);

        return convertTT_TDB(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCB_SCALE>
    TimeConverter<RealType, DutContainer>::convertUT1_TCB(const Time<RealType, TimeScale::UT1_SCALE> &ut1) const {

        const Time<RealType, TimeScale::TDB_SCALE> tdb = convertUT1_TDB(ut1);

        return convertTDB_TCG(tdb);
    }

    /**************************************************************************************************************\
                                                    UTC в другие шкалы
    \**************************************************************************************************************/


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TAI_SCALE>
    TimeConverter<RealType, DutContainer>::convertUTC_TAI(const Time<RealType, TimeScale::UTC_SCALE> &utc) const {

        RealType jdIntTAI, jdFracTAI;

        const int status = iauUtctai(utc.jdDayInt(), utc.jdDayFrac(), &jdIntTAI, &jdFracTAI);

        switch (status) {
            case 1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: DUBIOUS YEAR");

            case -1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNACCEPTABLE DATE");

            default:
                return Time<RealType, TimeScale::TAI_SCALE>(jdIntTAI, jdFracTAI);
        }
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UT1_SCALE>
    TimeConverter<RealType, DutContainer>::convertUTC_UT1(const Time<RealType, TimeScale::UTC_SCALE> &utc) const {

        RealType jdIntUT1, jdFracUT1;

        const int status = iauUtcut1(utc.jdDayInt(), utc.jdDayFrac(), dutContainer_.dut(utc), &jdIntUT1, &jdFracUT1);

        switch (status) {
            case 1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: DUBIOUS YEAR");

            case -1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNACCEPTABLE DATE");

            default:
                return Time<RealType, TimeScale::UT1_SCALE>(jdIntUT1, jdFracUT1);
        }
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TT_SCALE>
    TimeConverter<RealType, DutContainer>::convertUTC_TT(const Time<RealType, TimeScale::UTC_SCALE> &utc) const {

        const Time<RealType, TimeScale::UT1_SCALE> ut1 = convertUTC_UT1(utc);

        return convertUT1_TT(ut1);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCG_SCALE>
    TimeConverter<RealType, DutContainer>::convertUTC_TCG(const Time<RealType, TimeScale::UTC_SCALE> &utc) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertUTC_TT(utc);

        return convertTT_TCG(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCB_SCALE>
    TimeConverter<RealType, DutContainer>::convertUTC_TCB(const Time<RealType, TimeScale::UTC_SCALE> &utc) const {

        const Time<RealType, TimeScale::TDB_SCALE> tdb = convertUTC_TDB(utc);

        return convertTDB_TCB(tdb);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TDB_SCALE>
    TimeConverter<RealType, DutContainer>::convertUTC_TDB(const Time<RealType, TimeScale::UTC_SCALE> &utc) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertUTC_TT(utc);

        return convertTT_TDB(tt);
    }


    /**************************************************************************************************************\
                                                    TAI в другие шкалы
    \**************************************************************************************************************/

    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UTC_SCALE> TimeConverter<RealType, DutContainer>::convertTAI_UTC(
            const Time<RealType, TimeScale::TAI_SCALE> &tai) const {

        RealType jdIntTAI, jdFracTAI;

        const int status = iauTaiutc(tai.jdDayInt(), tai.jdDayFrac(), &jdIntTAI, &jdFracTAI);

        switch (status) {
            case 1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: DUBIOUS YEAR");

            case -1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNACCEPTABLE DATE");

            default:
                return Time<RealType, TimeScale::UTC_SCALE>(jdIntTAI, jdFracTAI);
        }
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UT1_SCALE> TimeConverter<RealType, DutContainer>::convertTAI_UT1(
            const Time<RealType, TimeScale::TAI_SCALE> &tai) const {

        Time<RealType, TimeScale::UTC_SCALE> utc = convertTAI_UTC(tai);

        return convertUTC_UT1(utc);

    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TT_SCALE>
    TimeConverter<RealType, DutContainer>::convertTAI_TT(const Time<RealType, TimeScale::TAI_SCALE> &tai) const {

        RealType jdIntTT, jdFracTT;

        const int status = iauTaitt(tai.jdDayInt(), tai.jdDayFrac(), &jdIntTT, &jdFracTT);

        switch (status) {
            case 0:
                return Time<RealType, TimeScale::TT_SCALE>(jdIntTT, jdFracTT);

            default:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCG_SCALE>
    TimeConverter<RealType, DutContainer>::convertTAI_TCG(const Time<RealType, TimeScale::TAI_SCALE> &tai) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convetTAI_TT(tai);

        return convertTT_TCG(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCB_SCALE>
    TimeConverter<RealType, DutContainer>::convertTAI_TCB(const Time<RealType, TimeScale::TAI_SCALE> &tai) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convetTAI_TT(tai);

        return convertTT_TCB(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TDB_SCALE>
    TimeConverter<RealType, DutContainer>::convertTAI_TDB(const Time<RealType, TimeScale::TAI_SCALE> &tai) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convetTAI_TT(tai);

        return convertTT_TDB(tt);
    }

    /**************************************************************************************************************\
                                                    TT в другие шкалы
    \**************************************************************************************************************/

    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UTC_SCALE>
    TimeConverter<RealType, DutContainer>::convertTT_UTC(const Time<RealType, TimeScale::TT_SCALE> &tt) const {

        const Time<RealType, TimeScale::TAI_SCALE> tai = convertTT_TAI(tt);

        return convertTAI_UTC(tai);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UT1_SCALE>
    TimeConverter<RealType, DutContainer>::convertTT_UT1(const Time<RealType, TimeScale::TT_SCALE> &tt) const {

        const Time<RealType, TimeScale::UTC_SCALE> utc = convertTT_UTC(tt);

        return convertUTC_UT1(utc);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TAI_SCALE>
    TimeConverter<RealType, DutContainer>::convertTT_TAI(const Time<RealType, TimeScale::TT_SCALE> &tt) const {

        RealType jdIntTAI, jdFracTAI;

        const int status = iauTttai(tt.jdDayInt(), tt.jdDayFrac(), &jdIntTAI, &jdFracTAI);

        switch (status) {
            case 0:
                return Time<RealType, TimeScale::TAI_SCALE>(jdIntTAI, jdFracTAI);

            default:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCG_SCALE>
    TimeConverter<RealType, DutContainer>::convertTT_TCG(const Time<RealType, TimeScale::TT_SCALE> &tt) const {

        RealType jdIntTCG, jdFracTCG;

        const int status = iauTttcg(tt.jdDayInt(), tt.jdDayFrac(), &jdIntTCG, &jdFracTCG);

        switch (status) {
            case 0:
                return Time<RealType, TimeScale::TCG_SCALE>(jdIntTCG, jdFracTCG);

            default:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCB_SCALE>
    TimeConverter<RealType, DutContainer>::convertTT_TCB(const Time<RealType, TimeScale::TT_SCALE> &tt) const {

        const Time<RealType, TimeScale::TDB_SCALE> tdb = convertTT_TDB(tt);

        return convertTDB_TCB(tdb);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TDB_SCALE>
    TimeConverter<RealType, DutContainer>::convertTT_TDB(const Time<RealType, TimeScale::TT_SCALE> &tt) const {

        const RealType g = 6.24 + 0.017202 * tt.jd() - 2451545;

        return tt + 0.0016 * std::sin(g);
    }


    /**************************************************************************************************************\
                                                    TCG в другие шкалы
    \**************************************************************************************************************/

    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UTC_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCG_UTC(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertTCG_TT(tcg);

        return convertTT_UTC(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UT1_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCG_UT1(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertTCG_TT(tcg);

        return convertTT_UT1(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TAI_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCG_TAI(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertTCG_TT(tcg);

        return convertTT_TAI(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TT_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCG_TT(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const {

        RealType jdIntTT, jdFracTT;

        const int status = iauTcgtt(tcg.jdDayInt(), tcg.jdDayFrac(), &jdIntTT, &jdFracTT);

        switch (status) {
            case 0:
                return Time<RealType, TimeScale::TT_SCALE>(jdIntTT, jdFracTT);

            default:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCB_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCG_TCB(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertTCG_TT(tcg);

        return convertTT_TCB(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TDB_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCG_TDB(const Time<RealType, TimeScale::TCG_SCALE> &tcg) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertTCG_TT(tcg);

        return convertTT_TDB(tt);
    }

    /**************************************************************************************************************\
                                                    TCB в другие шкалы
    \**************************************************************************************************************/

    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UTC_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCB_UTC(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const {

        const Time<RealType, TimeScale::TDB_SCALE> tdb = convertTCB_TDB(tcb);

        return convertTDB_UTC(tdb);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UT1_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCB_UT1(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const {

        const Time<RealType, TimeScale::TDB_SCALE> tdb = convertTCB_TDB(tcb);

        return convertTDB_UT1(tdb);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TAI_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCB_TAI(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const {

        const Time<RealType, TimeScale::TDB_SCALE> tdb = convertTCB_TDB(tcb);

        return convertTDB_TAI(tdb);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TT_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCB_TT(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const {

        const Time<RealType, TimeScale::TDB_SCALE> tdb = convertTCB_TDB(tcb);

        return convertTDB_TT(tdb);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCG_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCB_TCG(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const {

        const Time<RealType, TimeScale::TDB_SCALE> tdb = convertTCB_TDB(tcb);

        return convertTDB_TCG(tdb);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TDB_SCALE>
    TimeConverter<RealType, DutContainer>::convertTCB_TDB(const Time<RealType, TimeScale::TCB_SCALE> &tcb) const {

        RealType jdIntTDB, jdFracTDB;

        const int status = iauTcbtdb(tcb.jdDayInt(), tcb.jdDayFrac(), &jdIntTDB, &jdFracTDB);

        switch (status) {
            case 0:
                return Time<RealType, TimeScale::TDB_SCALE>(jdIntTDB, jdFracTDB);

            default:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }
    }

    /**************************************************************************************************************\
                                                TAI в другие шкалы
    \**************************************************************************************************************/

    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UTC_SCALE>
    TimeConverter<RealType, DutContainer>::convertTDB_UTC(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertTCB_TDB(tdb);

        return convertTT_UTC(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::UT1_SCALE>
    TimeConverter<RealType, DutContainer>::convertTDB_UT1(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertTCB_TDB(tdb);

        return convertTT_UT1(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TAI_SCALE>
    TimeConverter<RealType, DutContainer>::convertTDB_TAI(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertTCB_TDB(tdb);

        return convertTT_TAI(tt);
    }


    //TODO: здесь МПИ, не протестировано!!!
    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TT_SCALE>
    TimeConverter<RealType, DutContainer>::convertTDB_TT(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const {

        Time<RealType, TimeScale::TT_SCALE> tt;
        tt.buildFromJD(tdb.mjd());

        for (int i = 0; i < 3; ++i) {

            const RealType g = 6.24 + 0.017202 * tt.jd() - 2451545;
            tt = tdb - 0.0016 * std::sin(g);;
        }

        return tt;

    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCG_SCALE>
    TimeConverter<RealType, DutContainer>::convertTDB_TCG(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const {

        const Time<RealType, TimeScale::TT_SCALE> tt = convertTDB_TT(tdb);

        return convertTT_TCG(tt);
    }


    template<typename RealType, typename DutContainer>
    Time<RealType, TimeScale::TCB_SCALE>
    TimeConverter<RealType, DutContainer>::convertTDB_TCB(const Time<RealType, TimeScale::TDB_SCALE> &tdb) const {

        RealType jdIntTCB, jdFracTCB;

        const int status = iauTdbtcb(tdb.jdDayInt(), tdb.jdDayFrac(), &jdIntTCB, &jdFracTCB);

        switch (status) {
            case 0:
                return Time<RealType, TimeScale::TCB_SCALE>(jdIntTCB, jdFracTCB);

            default:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define UNITE_4(A, B, C, D) A##B##C##D

#define CREATE_ONE_SCALE(FROM) \
    if constexpr (To == TimeScale::TT_SCALE) {         \
        return UNITE_4(convert, FROM, _, TT)(time);    \
    } else if constexpr (To == TimeScale::UTC_SCALE) { \
        return UNITE_4(convert, FROM, _, UTC)(time);   \
    } else if constexpr (To == TimeScale::UT1_SCALE) { \
        return UNITE_4(convert, FROM, _, UT1)(time);   \
    } else if constexpr (To == TimeScale::TAI_SCALE) { \
        return UNITE_4(convert, FROM, _, TAI)(time);   \
    } else if constexpr (To == TimeScale::TDB_SCALE) { \
        return UNITE_4(convert, FROM, _, TDB)(time);   \
    } else if constexpr (To == TimeScale::TCB_SCALE) { \
        return UNITE_4(convert, FROM, _, TCB)(time);   \
    } else if constexpr (To == TimeScale::TCG_SCALE) { \
        return UNITE_4(convert, FROM, _, TCG)(time);   \
    } else {                                           \
        static_assert();                               \
    }

    template<typename RealType, typename DutContainer>
    template<TimeScale To, TimeScale From>
    Time<RealType, To> TimeConverter<RealType, DutContainer>::convert(const Time<RealType, From> &from) const {
        if constexpr (From == To) {
            return time;
        } else if constexpr (From == TimeScale::TT_SCALE) {
            CREATE_ONE_SCALE(TT)
        } else if constexpr (From == TimeScale::UTC_SCALE) {
            CREATE_ONE_SCALE(UTC)
        } else if constexpr (From == TimeScale::UT1_SCALE) {
            CREATE_ONE_SCALE(UT1)
        } else if constexpr (From == TimeScale::TAI_SCALE) {
            CREATE_ONE_SCALE(TAI)
        } else if constexpr (From == TimeScale::TDB_SCALE) {
            CREATE_ONE_SCALE(TDB)
        } else if constexpr (From == TimeScale::TCB_SCALE) {
            CREATE_ONE_SCALE(TCB)
        } else if constexpr (From == TimeScale::TCG_SCALE) {
            CREATE_ONE_SCALE(TCG)
        } else {
            static_assert(AlwaysFalse<From>);
        }
    }

#undef UNITE_4
#undef CREATE_ONE_SCALE

}

#endif //BALLISTICS2023_TIMECONVERTER_H
