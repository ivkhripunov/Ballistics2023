//
// Created by ivankhripunov on 30.11.23.
//

#ifndef BALLISTICS2023_TIMECONVERTER_H
#define BALLISTICS2023_TIMECONVERTER_H

#include "Time.h"

//1. NRVO? в конвертации

//Заметки
//При реализации конвертации через софу реализуем только переход на соседнюю шкалу. Все остальное используем уже "готовое"
//При некоторых переходах необходимо решать нелинейное уравнение. Решаем с помощью МПИ

//TODO: brace initializer

namespace Ballistics::TimeModule {

    using TimeScale = Ballistics::TimeModule::TimeScale;


    template<typename DutContainer>
    class TimeConverter {
        DutContainer dutContainer_;

        //это финт ушами, чтобы "обмануть компилятор. В макросе компилятор сделает статик ассерт в самом начале,
        //несмотря на else. Это приведет к тому, что, если подать туда false, то программа будет всегда падать на
        //этапе компиляции. С таким финтом мы можем гарантировать, что программа упадет только если дойдет до этого else
        template<TimeScale Scale>
        static constexpr bool AlwaysFalse = false;

        [[nodiscard]] scalar dtr(const Time<TimeScale::TT_SCALE> &tt) const noexcept;

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
        Time<To> convert(const Time<From> &from) const;

        /**************************************************************************************************************\
                                                    UT1 в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<TimeScale::UTC_SCALE>
        convertUT1_UTC(const Time<TimeScale::UT1_SCALE> &ut1) const;

        [[nodiscard]] Time<TimeScale::TAI_SCALE>
        convertUT1_TAI(const Time<TimeScale::UT1_SCALE> &ut1) const;

        [[nodiscard]] Time<TimeScale::TT_SCALE>
        convertUT1_TT(const Time<TimeScale::UT1_SCALE> &ut1) const;

        [[nodiscard]] Time<TimeScale::TCG_SCALE>
        convertUT1_TCG(const Time<TimeScale::UT1_SCALE> &ut1) const;

        [[nodiscard]] Time<TimeScale::TDB_SCALE>
        convertUT1_TDB(const Time<TimeScale::UT1_SCALE> &ut1) const;

        [[nodiscard]] Time<TimeScale::TCB_SCALE>
        convertUT1_TCB(const Time<TimeScale::UT1_SCALE> &ut1) const;

        /**************************************************************************************************************\
                                                    UTC в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<TimeScale::TAI_SCALE>
        convertUTC_TAI(const Time<TimeScale::UTC_SCALE> &utc) const;

        [[nodiscard]] Time<TimeScale::UT1_SCALE>
        convertUTC_UT1(const Time<TimeScale::UTC_SCALE> &utc) const;

        [[nodiscard]] Time<TimeScale::TT_SCALE>
        convertUTC_TT(const Time<TimeScale::UTC_SCALE> &utc) const;

        [[nodiscard]] Time<TimeScale::TCG_SCALE>
        convertUTC_TCG(const Time<TimeScale::UTC_SCALE> &utc) const;

        [[nodiscard]] Time<TimeScale::TCB_SCALE>
        convertUTC_TCB(const Time<TimeScale::UTC_SCALE> &utc) const;

        [[nodiscard]] Time<TimeScale::TDB_SCALE>
        convertUTC_TDB(const Time<TimeScale::UTC_SCALE> &utc) const;

        /**************************************************************************************************************\
                                                    TAI в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<TimeScale::UTC_SCALE>
        convertTAI_UTC(const Time<TimeScale::TAI_SCALE> &tai) const;

        [[nodiscard]] Time<TimeScale::UT1_SCALE>
        convertTAI_UT1(const Time<TimeScale::TAI_SCALE> &tai) const;

        [[nodiscard]] Time<TimeScale::TT_SCALE>
        convertTAI_TT(const Time<TimeScale::TAI_SCALE> &tai) const;

        [[nodiscard]] Time<TimeScale::TCG_SCALE>
        convertTAI_TCG(const Time<TimeScale::TAI_SCALE> &tai) const;

        [[nodiscard]] Time<TimeScale::TCB_SCALE>
        convertTAI_TCB(const Time<TimeScale::TAI_SCALE> &tai) const;

        [[nodiscard]] Time<TimeScale::TDB_SCALE>
        convertTAI_TDB(const Time<TimeScale::TAI_SCALE> &tai) const;

        /**************************************************************************************************************\
                                                    TT в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<TimeScale::UTC_SCALE>
        convertTT_UTC(const Time<TimeScale::TT_SCALE> &tt) const;

        [[nodiscard]] Time<TimeScale::UT1_SCALE>
        convertTT_UT1(const Time<TimeScale::TT_SCALE> &tt) const;

        [[nodiscard]] Time<TimeScale::TAI_SCALE>
        convertTT_TAI(const Time<TimeScale::TT_SCALE> &tt) const;

        [[nodiscard]] Time<TimeScale::TCG_SCALE>
        convertTT_TCG(const Time<TimeScale::TT_SCALE> &tt) const;

        [[nodiscard]] Time<TimeScale::TCB_SCALE>
        convertTT_TCB(const Time<TimeScale::TT_SCALE> &tt) const;

        [[nodiscard]] Time<TimeScale::TDB_SCALE>
        convertTT_TDB(const Time<TimeScale::TT_SCALE> &tt) const;

        /**************************************************************************************************************\
                                                    TCG в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<TimeScale::UTC_SCALE>
        convertTCG_UTC(const Time<TimeScale::TCG_SCALE> &tcg) const;

        [[nodiscard]] Time<TimeScale::UT1_SCALE>
        convertTCG_UT1(const Time<TimeScale::TCG_SCALE> &tcg) const;

        [[nodiscard]] Time<TimeScale::TAI_SCALE>
        convertTCG_TAI(const Time<TimeScale::TCG_SCALE> &tcg) const;

        [[nodiscard]] Time<TimeScale::TT_SCALE>
        convertTCG_TT(const Time<TimeScale::TCG_SCALE> &tcg) const;

        [[nodiscard]] Time<TimeScale::TCB_SCALE>
        convertTCG_TCB(const Time<TimeScale::TCG_SCALE> &tcg) const;

        [[nodiscard]] Time<TimeScale::TDB_SCALE>
        convertTCG_TDB(const Time<TimeScale::TCG_SCALE> &tcg) const;

        /**************************************************************************************************************\
                                                    TCB в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<TimeScale::UTC_SCALE>
        convertTCB_UTC(const Time<TimeScale::TCB_SCALE> &tcb) const;

        [[nodiscard]] Time<TimeScale::UT1_SCALE>
        convertTCB_UT1(const Time<TimeScale::TCB_SCALE> &tcb) const;

        [[nodiscard]] Time<TimeScale::TAI_SCALE>
        convertTCB_TAI(const Time<TimeScale::TCB_SCALE> &tcb) const;

        [[nodiscard]] Time<TimeScale::TT_SCALE>
        convertTCB_TT(const Time<TimeScale::TCB_SCALE> &tcb) const;

        [[nodiscard]] Time<TimeScale::TCG_SCALE>
        convertTCB_TCG(const Time<TimeScale::TCB_SCALE> &tcb) const;

        [[nodiscard]] Time<TimeScale::TDB_SCALE>
        convertTCB_TDB(const Time<TimeScale::TCB_SCALE> &tcb) const;

        /**************************************************************************************************************\
                                                    TDB в другие шкалы
        \**************************************************************************************************************/

        [[nodiscard]] Time<TimeScale::UTC_SCALE>
        convertTDB_UTC(const Time<TimeScale::TDB_SCALE> &tdb) const;

        [[nodiscard]] Time<TimeScale::UT1_SCALE>
        convertTDB_UT1(const Time<TimeScale::TDB_SCALE> &tdb) const;

        [[nodiscard]] Time<TimeScale::TAI_SCALE>
        convertTDB_TAI(const Time<TimeScale::TDB_SCALE> &tdb) const;

        [[nodiscard]] Time<TimeScale::TT_SCALE>
        convertTDB_TT(const Time<TimeScale::TDB_SCALE> &tdb) const;

        [[nodiscard]] Time<TimeScale::TCG_SCALE>
        convertTDB_TCG(const Time<TimeScale::TDB_SCALE> &tdb) const;

        [[nodiscard]] Time<TimeScale::TCB_SCALE>
        convertTDB_TCB(const Time<TimeScale::TDB_SCALE> &tdb) const;

    };

    template<typename DutContainer>
    scalar TimeConverter<DutContainer>::dtr(const Time<TimeScale::TT_SCALE> &tt) const noexcept {
//        const scalar g = static_cast<scalar>(6.24) + static_cast<scalar>(0.017202) *
//                                                     ((tt.jdDayInt() - static_cast<scalar>(2451545)) + tt.jdDayFrac());
//        const scalar sing = std::sin(g);
//        return static_cast<scalar>(0.001657) * sing;

//SOFA recommended accuracy up to 50 microseconds
        return static_cast<scalar>(iauDtdb(tt.jdDayInt(), tt.jdDayFrac(), 0, 0, 0, 0));
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    template<typename DutContainer>
    TimeConverter<DutContainer>::TimeConverter(const DutContainer &dutContainer) noexcept : dutContainer_(
            dutContainer) {}

    /**************************************************************************************************************\
                                                    UT1 в другие шкалы
    \**************************************************************************************************************/
    //здесь МПИ!!!
    template<typename DutContainer>
    Time<TimeScale::UTC_SCALE>
    TimeConverter<DutContainer>::convertUT1_UTC(const Time<TimeScale::UT1_SCALE> &ut1) const {

        scalar jdIntUTC = ut1.jdDayInt(), jdFracUTC = ut1.jdDayFrac();
        for (indexType i = 0; i < 3; ++i) {
            const int status = iauUt1utc(ut1.jdDayInt(), ut1.jdDayFrac(),
                                         dutContainer_.dut(Time<TimeScale::UTC_SCALE>::buildFromMJD(
                                                 jdIntUTC + jdFracUTC - static_cast<scalar>(2400000.5))),
                                         &jdIntUTC,
                                         &jdFracUTC);

            if (status != 0) {
                throw Ballistics::Exceptions::TimeModuleException("SOFA ERROR");
            }
        }

        return Time<TimeScale::UTC_SCALE>(jdIntUTC, jdFracUTC);
    }


    template<typename DutContainer>
    Time<TimeScale::TAI_SCALE>
    TimeConverter<DutContainer>::convertUT1_TAI(const Time<TimeScale::UT1_SCALE> &ut1) const {

        const Time<TimeScale::UTC_SCALE> utc = convertUT1_UTC(ut1);

        return convertUTC_TAI(utc);
    }


    template<typename DutContainer>
    Time<TimeScale::TT_SCALE>
    TimeConverter<DutContainer>::convertUT1_TT(const Time<TimeScale::UT1_SCALE> &ut1) const {

        const Time<TimeScale::TAI_SCALE> tai = convertUT1_TAI(ut1);

        return convertTAI_TT(tai);
    }


    template<typename DutContainer>
    Time<TimeScale::TCG_SCALE>
    TimeConverter<DutContainer>::convertUT1_TCG(const Time<TimeScale::UT1_SCALE> &ut1) const {

        const Time<TimeScale::TT_SCALE> tt = convertUT1_TT(ut1);

        return convertTT_TCG(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TDB_SCALE>
    TimeConverter<DutContainer>::convertUT1_TDB(const Time<TimeScale::UT1_SCALE> &ut1) const {

        const Time<TimeScale::TT_SCALE> tt = convertUT1_TT(ut1);

        return convertTT_TDB(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TCB_SCALE>
    TimeConverter<DutContainer>::convertUT1_TCB(const Time<TimeScale::UT1_SCALE> &ut1) const {

        const Time<TimeScale::TDB_SCALE> tdb = convertUT1_TDB(ut1);

        return convertTDB_TCG(tdb);
    }

    /**************************************************************************************************************\
                                                    UTC в другие шкалы
    \**************************************************************************************************************/


    template<typename DutContainer>
    Time<TimeScale::TAI_SCALE>
    TimeConverter<DutContainer>::convertUTC_TAI(const Time<TimeScale::UTC_SCALE> &utc) const {

        scalar jdIntTAI, jdFracTAI;

        const int status = iauUtctai(utc.jdDayInt(), utc.jdDayFrac(), &jdIntTAI, &jdFracTAI);

        switch (status) {
            case 1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: DUBIOUS YEAR");

            case -1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNACCEPTABLE DATE");

            default:
                return Time<TimeScale::TAI_SCALE>(jdIntTAI, jdFracTAI);
        }
    }


    template<typename DutContainer>
    Time<TimeScale::UT1_SCALE>
    TimeConverter<DutContainer>::convertUTC_UT1(const Time<TimeScale::UTC_SCALE> &utc) const {

        scalar jdIntUT1, jdFracUT1;

        const int status = iauUtcut1(utc.jdDayInt(), utc.jdDayFrac(), dutContainer_.dut(utc), &jdIntUT1, &jdFracUT1);

        switch (status) {
            case 1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: DUBIOUS YEAR");

            case -1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNACCEPTABLE DATE");

            default:
                return Time<TimeScale::UT1_SCALE>(jdIntUT1, jdFracUT1);
        }
    }


    template<typename DutContainer>
    Time<TimeScale::TT_SCALE>
    TimeConverter<DutContainer>::convertUTC_TT(const Time<TimeScale::UTC_SCALE> &utc) const {

        const Time<TimeScale::UT1_SCALE> ut1 = convertUTC_UT1(utc);

        return convertUT1_TT(ut1);
    }


    template<typename DutContainer>
    Time<TimeScale::TCG_SCALE>
    TimeConverter<DutContainer>::convertUTC_TCG(const Time<TimeScale::UTC_SCALE> &utc) const {

        const Time<TimeScale::TT_SCALE> tt = convertUTC_TT(utc);

        return convertTT_TCG(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TCB_SCALE>
    TimeConverter<DutContainer>::convertUTC_TCB(const Time<TimeScale::UTC_SCALE> &utc) const {

        const Time<TimeScale::TDB_SCALE> tdb = convertUTC_TDB(utc);

        return convertTDB_TCB(tdb);
    }


    template<typename DutContainer>
    Time<TimeScale::TDB_SCALE>
    TimeConverter<DutContainer>::convertUTC_TDB(const Time<TimeScale::UTC_SCALE> &utc) const {

        const Time<TimeScale::TT_SCALE> tt = convertUTC_TT(utc);

        return convertTT_TDB(tt);
    }


    /**************************************************************************************************************\
                                                    TAI в другие шкалы
    \**************************************************************************************************************/

    template<typename DutContainer>
    Time<TimeScale::UTC_SCALE> TimeConverter<DutContainer>::convertTAI_UTC(
            const Time<TimeScale::TAI_SCALE> &tai) const {

        scalar jdIntTAI, jdFracTAI;

        const int status = iauTaiutc(tai.jdDayInt(), tai.jdDayFrac(), &jdIntTAI, &jdFracTAI);

        switch (status) {
            case 1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: DUBIOUS YEAR");

            case -1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNACCEPTABLE DATE");

            default:
                return Time<TimeScale::UTC_SCALE>(jdIntTAI, jdFracTAI);
        }
    }


    template<typename DutContainer>
    Time<TimeScale::UT1_SCALE> TimeConverter<DutContainer>::convertTAI_UT1(
            const Time<TimeScale::TAI_SCALE> &tai) const {

        Time<TimeScale::UTC_SCALE> utc = convertTAI_UTC(tai);

        return convertUTC_UT1(utc);

    }


    template<typename DutContainer>
    Time<TimeScale::TT_SCALE>
    TimeConverter<DutContainer>::convertTAI_TT(const Time<TimeScale::TAI_SCALE> &tai) const {

        scalar jdIntTT, jdFracTT;

        const int status = iauTaitt(tai.jdDayInt(), tai.jdDayFrac(), &jdIntTT, &jdFracTT);

        switch (status) {
            case 0:
                return Time<TimeScale::TT_SCALE>(jdIntTT, jdFracTT);

            default:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }
    }


    template<typename DutContainer>
    Time<TimeScale::TCG_SCALE>
    TimeConverter<DutContainer>::convertTAI_TCG(const Time<TimeScale::TAI_SCALE> &tai) const {

        const Time<TimeScale::TT_SCALE> tt = convertTAI_TT(tai);

        return convertTT_TCG(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TCB_SCALE>
    TimeConverter<DutContainer>::convertTAI_TCB(const Time<TimeScale::TAI_SCALE> &tai) const {

        const Time<TimeScale::TT_SCALE> tt = convertTAI_TT(tai);

        return convertTT_TCB(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TDB_SCALE>
    TimeConverter<DutContainer>::convertTAI_TDB(const Time<TimeScale::TAI_SCALE> &tai) const {

        const Time<TimeScale::TT_SCALE> tt = convertTAI_TT(tai);

        return convertTT_TDB(tt);
    }

    /**************************************************************************************************************\
                                                    TT в другие шкалы
    \**************************************************************************************************************/

    template<typename DutContainer>
    Time<TimeScale::UTC_SCALE>
    TimeConverter<DutContainer>::convertTT_UTC(const Time<TimeScale::TT_SCALE> &tt) const {

        const Time<TimeScale::TAI_SCALE> tai = convertTT_TAI(tt);

        return convertTAI_UTC(tai);
    }


    template<typename DutContainer>
    Time<TimeScale::UT1_SCALE>
    TimeConverter<DutContainer>::convertTT_UT1(const Time<TimeScale::TT_SCALE> &tt) const {

        const Time<TimeScale::UTC_SCALE> utc = convertTT_UTC(tt);

        return convertUTC_UT1(utc);
    }


    template<typename DutContainer>
    Time<TimeScale::TAI_SCALE>
    TimeConverter<DutContainer>::convertTT_TAI(const Time<TimeScale::TT_SCALE> &tt) const {

        scalar jdIntTAI, jdFracTAI;

        const int status = iauTttai(tt.jdDayInt(), tt.jdDayFrac(), &jdIntTAI, &jdFracTAI);

        switch (status) {
            case 0:
                return Time<TimeScale::TAI_SCALE>(jdIntTAI, jdFracTAI);

            default:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }
    }


    template<typename DutContainer>
    Time<TimeScale::TCG_SCALE>
    TimeConverter<DutContainer>::convertTT_TCG(const Time<TimeScale::TT_SCALE> &tt) const {

        scalar jdIntTCG, jdFracTCG;

        const int status = iauTttcg(tt.jdDayInt(), tt.jdDayFrac(), &jdIntTCG, &jdFracTCG);

        switch (status) {
            case 0:
                return Time<TimeScale::TCG_SCALE>(jdIntTCG, jdFracTCG);

            default:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }
    }


    template<typename DutContainer>
    Time<TimeScale::TCB_SCALE>
    TimeConverter<DutContainer>::convertTT_TCB(const Time<TimeScale::TT_SCALE> &tt) const {

        const Time<TimeScale::TDB_SCALE> tdb = convertTT_TDB(tt);

        return convertTDB_TCB(tdb);
    }


    template<typename DutContainer>
    Time<TimeScale::TDB_SCALE>
    TimeConverter<DutContainer>::convertTT_TDB(const Time<TimeScale::TT_SCALE> &tt) const {

        scalar jdIntTDB, jdFracTDB;
        const int status = iauTttdb(tt.jdDayInt(), tt.jdDayFrac(), dtr(tt), &jdIntTDB, &jdFracTDB);

        if (status != 0) {
            throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }

        return {jdIntTDB, jdFracTDB};
    }


    /**************************************************************************************************************\
                                                    TCG в другие шкалы
    \**************************************************************************************************************/

    template<typename DutContainer>
    Time<TimeScale::UTC_SCALE>
    TimeConverter<DutContainer>::convertTCG_UTC(const Time<TimeScale::TCG_SCALE> &tcg) const {

        const Time<TimeScale::TT_SCALE> tt = convertTCG_TT(tcg);

        return convertTT_UTC(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::UT1_SCALE>
    TimeConverter<DutContainer>::convertTCG_UT1(const Time<TimeScale::TCG_SCALE> &tcg) const {

        const Time<TimeScale::TT_SCALE> tt = convertTCG_TT(tcg);

        return convertTT_UT1(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TAI_SCALE>
    TimeConverter<DutContainer>::convertTCG_TAI(const Time<TimeScale::TCG_SCALE> &tcg) const {

        const Time<TimeScale::TT_SCALE> tt = convertTCG_TT(tcg);

        return convertTT_TAI(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TT_SCALE>
    TimeConverter<DutContainer>::convertTCG_TT(const Time<TimeScale::TCG_SCALE> &tcg) const {

        scalar jdIntTT, jdFracTT;

        const int status = iauTcgtt(tcg.jdDayInt(), tcg.jdDayFrac(), &jdIntTT, &jdFracTT);

        switch (status) {
            case 0:
                return Time<TimeScale::TT_SCALE>(jdIntTT, jdFracTT);

            default:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }
    }


    template<typename DutContainer>
    Time<TimeScale::TCB_SCALE>
    TimeConverter<DutContainer>::convertTCG_TCB(const Time<TimeScale::TCG_SCALE> &tcg) const {

        const Time<TimeScale::TT_SCALE> tt = convertTCG_TT(tcg);

        return convertTT_TCB(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TDB_SCALE>
    TimeConverter<DutContainer>::convertTCG_TDB(const Time<TimeScale::TCG_SCALE> &tcg) const {

        const Time<TimeScale::TT_SCALE> tt = convertTCG_TT(tcg);

        return convertTT_TDB(tt);
    }

    /**************************************************************************************************************\
                                                    TCB в другие шкалы
    \**************************************************************************************************************/

    template<typename DutContainer>
    Time<TimeScale::UTC_SCALE>
    TimeConverter<DutContainer>::convertTCB_UTC(const Time<TimeScale::TCB_SCALE> &tcb) const {

        const Time<TimeScale::TDB_SCALE> tdb = convertTCB_TDB(tcb);

        return convertTDB_UTC(tdb);
    }


    template<typename DutContainer>
    Time<TimeScale::UT1_SCALE>
    TimeConverter<DutContainer>::convertTCB_UT1(const Time<TimeScale::TCB_SCALE> &tcb) const {

        const Time<TimeScale::TDB_SCALE> tdb = convertTCB_TDB(tcb);

        return convertTDB_UT1(tdb);
    }


    template<typename DutContainer>
    Time<TimeScale::TAI_SCALE>
    TimeConverter<DutContainer>::convertTCB_TAI(const Time<TimeScale::TCB_SCALE> &tcb) const {

        const Time<TimeScale::TDB_SCALE> tdb = convertTCB_TDB(tcb);

        return convertTDB_TAI(tdb);
    }


    template<typename DutContainer>
    Time<TimeScale::TT_SCALE>
    TimeConverter<DutContainer>::convertTCB_TT(const Time<TimeScale::TCB_SCALE> &tcb) const {

        const Time<TimeScale::TDB_SCALE> tdb = convertTCB_TDB(tcb);

        return convertTDB_TT(tdb);
    }


    template<typename DutContainer>
    Time<TimeScale::TCG_SCALE>
    TimeConverter<DutContainer>::convertTCB_TCG(const Time<TimeScale::TCB_SCALE> &tcb) const {

        const Time<TimeScale::TDB_SCALE> tdb = convertTCB_TDB(tcb);

        return convertTDB_TCG(tdb);
    }


    template<typename DutContainer>
    Time<TimeScale::TDB_SCALE>
    TimeConverter<DutContainer>::convertTCB_TDB(const Time<TimeScale::TCB_SCALE> &tcb) const {

        scalar jdIntTDB, jdFracTDB;

        const int status = iauTcbtdb(tcb.jdDayInt(), tcb.jdDayFrac(), &jdIntTDB, &jdFracTDB);

        switch (status) {
            case 0:
                return Time<TimeScale::TDB_SCALE>(jdIntTDB, jdFracTDB);

            default:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }
    }

    /**************************************************************************************************************\
                                                TAI в другие шкалы
    \**************************************************************************************************************/

    template<typename DutContainer>
    Time<TimeScale::UTC_SCALE>
    TimeConverter<DutContainer>::convertTDB_UTC(const Time<TimeScale::TDB_SCALE> &tdb) const {

        const Time<TimeScale::TT_SCALE> tt = convertTCB_TDB(tdb);

        return convertTT_UTC(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::UT1_SCALE>
    TimeConverter<DutContainer>::convertTDB_UT1(const Time<TimeScale::TDB_SCALE> &tdb) const {

        const Time<TimeScale::TT_SCALE> tt = convertTCB_TDB(tdb);

        return convertTT_UT1(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TAI_SCALE>
    TimeConverter<DutContainer>::convertTDB_TAI(const Time<TimeScale::TDB_SCALE> &tdb) const {

        const Time<TimeScale::TT_SCALE> tt = convertTCB_TDB(tdb);

        return convertTT_TAI(tt);
    }


    //TODO: здесь МПИ, не протестировано!!!
    template<typename DutContainer>
    Time<TimeScale::TT_SCALE>
    TimeConverter<DutContainer>::convertTDB_TT(const Time<TimeScale::TDB_SCALE> &tdb) const {

        //TODO: неверное использование фабрики, возможно поправил, нужны тесты
        Time<TimeScale::TT_SCALE> tt = Time<TimeScale::TT_SCALE>::buildFromMJD(tdb.mjd());

        for (int i = 0; i < 3; ++i) {

            const scalar g = 6.24 + 0.017202 * tt.jd() - 2451545;
            //tt = tdb - 0.0016 * std::sin(g);;
        }

        return tt;

    }


    template<typename DutContainer>
    Time<TimeScale::TCG_SCALE>
    TimeConverter<DutContainer>::convertTDB_TCG(const Time<TimeScale::TDB_SCALE> &tdb) const {

        const Time<TimeScale::TT_SCALE> tt = convertTDB_TT(tdb);

        return convertTT_TCG(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TCB_SCALE>
    TimeConverter<DutContainer>::convertTDB_TCB(const Time<TimeScale::TDB_SCALE> &tdb) const {

        scalar jdIntTCB, jdFracTCB;

        const int status = iauTdbtcb(tdb.jdDayInt(), tdb.jdDayFrac(), &jdIntTCB, &jdFracTCB);

        switch (status) {
            case 0:
                return Time<TimeScale::TCB_SCALE>(jdIntTCB, jdFracTCB);

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

//    template<typename DutContainer>
//    template<TimeScale To, TimeScale From>
//    Time<To> TimeConverter<DutContainer>::convert(const Time<From> &from) const {
//        if constexpr (From == To) {
//            return time;
//        } else if constexpr (From == TimeScale::TT_SCALE) {
//            CREATE_ONE_SCALE(TT)
//        } else if constexpr (From == TimeScale::UTC_SCALE) {
//            CREATE_ONE_SCALE(UTC)
//        } else if constexpr (From == TimeScale::UT1_SCALE) {
//            CREATE_ONE_SCALE(UT1)
//        } else if constexpr (From == TimeScale::TAI_SCALE) {
//            CREATE_ONE_SCALE(TAI)
//        } else if constexpr (From == TimeScale::TDB_SCALE) {
//            CREATE_ONE_SCALE(TDB)
//        } else if constexpr (From == TimeScale::TCB_SCALE) {
//            CREATE_ONE_SCALE(TCB)
//        } else if constexpr (From == TimeScale::TCG_SCALE) {
//            CREATE_ONE_SCALE(TCG)
//        } else {
//            static_assert(AlwaysFalse<From>);
//        }
//    }

#undef UNITE_4
#undef CREATE_ONE_SCALE

}

#endif //BALLISTICS2023_TIMECONVERTER_H
