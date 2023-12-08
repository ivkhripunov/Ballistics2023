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

//TODO: добавить static_cast<double>() для передачи в функции софы

namespace Ballistics::TimeModule {

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

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template<typename DutContainer>
    scalar TimeConverter<DutContainer>::dtr(const Time<TimeScale::TT_SCALE> &tt) const noexcept {

        //SOFA recommended accuracy up to 50 microseconds, см кукбук стр 22
        return static_cast<scalar>(iauDtdb(static_cast<double >(tt.jdDayInt()), static_cast<double>(tt.jdDayFrac()), 0,
                                           0, 0, 0));
    }

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

        double jdIntUTC = ut1.jdDayInt(), jdFracUTC = ut1.jdDayFrac();
        for (indexType i = 0; i < 3; ++i) {

            const int status = iauUt1utc(static_cast<double>(ut1.jdDayInt()), static_cast<double>(ut1.jdDayFrac()),
                                         static_cast<double>(dutContainer_.dut(Time<TimeScale::UTC_SCALE>(jdIntUTC, jdFracUTC))),
                                         &jdIntUTC,
                                         &jdFracUTC);

            if (status != 0) {
                throw Ballistics::Exceptions::TimeModuleException("SOFA ERROR");
            }
        }

        return {static_cast<scalar>(jdIntUTC), static_cast<scalar>(jdFracUTC)};
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

        return convertTDB_TCB(tdb);
    }

    /**************************************************************************************************************\
                                                    UTC в другие шкалы
    \**************************************************************************************************************/


    template<typename DutContainer>
    Time<TimeScale::TAI_SCALE>
    TimeConverter<DutContainer>::convertUTC_TAI(const Time<TimeScale::UTC_SCALE> &utc) const {

        double jdIntTAI, jdFracTAI;

        const int status = iauUtctai(static_cast<double >(utc.jdDayInt()), static_cast<double >(utc.jdDayFrac()),
                                     &jdIntTAI, &jdFracTAI);

        switch (status) {
            case 1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: DUBIOUS YEAR");

            case -1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNACCEPTABLE DATE");

            default:
                return {static_cast<scalar>(jdIntTAI), static_cast<scalar>(jdFracTAI)};
        }
    }


    template<typename DutContainer>
    Time<TimeScale::UT1_SCALE>
    TimeConverter<DutContainer>::convertUTC_UT1(const Time<TimeScale::UTC_SCALE> &utc) const {

        double jdIntUT1, jdFracUT1;

        const int status = iauUtcut1(static_cast<double >(utc.jdDayInt()), static_cast<double >(utc.jdDayFrac()), dutContainer_.dut(utc), &jdIntUT1, &jdFracUT1);

        switch (status) {
            case 1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: DUBIOUS YEAR");

            case -1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNACCEPTABLE DATE");

            default:
                return {static_cast<scalar>(jdIntUT1), static_cast<scalar>(jdFracUT1)};
        }
    }


    template<typename DutContainer>
    Time<TimeScale::TT_SCALE>
    TimeConverter<DutContainer>::convertUTC_TT(const Time<TimeScale::UTC_SCALE> &utc) const {

        const Time<TimeScale::TAI_SCALE> tai = convertUTC_TAI(utc);

        return convertTAI_TT(tai);
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

        double jdIntTAI, jdFracTAI;

        const int status = iauTaiutc(static_cast<double >(tai.jdDayInt()), static_cast<double >(tai.jdDayFrac()), &jdIntTAI, &jdFracTAI);

        switch (status) {
            case 1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: DUBIOUS YEAR");

            case -1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNACCEPTABLE DATE");

            default:
                return {static_cast<scalar >(jdIntTAI), static_cast<scalar>(jdFracTAI)};
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

        if (status != 0) {
            throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }

        return {jdIntTT, jdFracTT};

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

        if (status != 0) {
            throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }

        return {jdIntTAI, jdFracTAI};

    }


    template<typename DutContainer>
    Time<TimeScale::TCG_SCALE>
    TimeConverter<DutContainer>::convertTT_TCG(const Time<TimeScale::TT_SCALE> &tt) const {

        scalar jdIntTCG, jdFracTCG;

        const int status = iauTttcg(tt.jdDayInt(), tt.jdDayFrac(), &jdIntTCG, &jdFracTCG);

        if (status != 0) {
            throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }

        return {jdIntTCG, jdFracTCG};
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

        if (status != 0) {
            throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNIDENTIFIED ERROR");
        }

        return {jdIntTT, jdFracTT};
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

        if (status != 0) {
            throw Ballistics::Exceptions::TimeModuleException("SOFA ERROR");
        }

        return {jdIntTDB, jdFracTDB};

    }

    /**************************************************************************************************************\
                                                TDB в другие шкалы
    \**************************************************************************************************************/

    template<typename DutContainer>
    Time<TimeScale::UTC_SCALE>
    TimeConverter<DutContainer>::convertTDB_UTC(const Time<TimeScale::TDB_SCALE> &tdb) const {

        const Time<TimeScale::TT_SCALE> tt = convertTDB_TT(tdb);

        return convertTT_UTC(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::UT1_SCALE>
    TimeConverter<DutContainer>::convertTDB_UT1(const Time<TimeScale::TDB_SCALE> &tdb) const {

        const Time<TimeScale::TT_SCALE> tt = convertTDB_TT(tdb);

        return convertTT_UT1(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TAI_SCALE>
    TimeConverter<DutContainer>::convertTDB_TAI(const Time<TimeScale::TDB_SCALE> &tdb) const {

        const Time<TimeScale::TT_SCALE> tt = convertTDB_TT(tdb);

        return convertTT_TAI(tt);
    }


    template<typename DutContainer>
    Time<TimeScale::TT_SCALE>
    TimeConverter<DutContainer>::convertTDB_TT(const Time<TimeScale::TDB_SCALE> &tdb) const {

        scalar jdIntTT = tdb.jdDayInt(), jdFracTT = tdb.jdDayFrac();
        for (indexType i = 0; i < 3; ++i) {
            const int status = iauTdbtt(tdb.jdDayInt(), tdb.jdDayFrac(),
                                        dtr(Time<TimeScale::TT_SCALE>(jdIntTT, jdFracTT)),
                                        &jdIntTT,
                                        &jdFracTT);

            if (status != 0) {
                throw Ballistics::Exceptions::TimeModuleException("SOFA ERROR");
            }
        }

        return {jdIntTT, jdFracTT};

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

        if (status != 0) {
            throw Ballistics::Exceptions::TimeModuleException("SOFA ERROR");
        }

        return {jdIntTCB, jdFracTCB};

    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define UNITE_4(A, B, C, D) A##B##C##D

#define CREATE_ONE_SCALE(SCALE_FROM)                             \
    if constexpr (ScaleTo == TimeScale::TT_SCALE) {         \
        return UNITE_4(convert, SCALE_FROM, _, TT)(time);       \
    } else if constexpr (ScaleTo == TimeScale::UTC_SCALE) { \
        return UNITE_4(convert, SCALE_FROM, _, UTC)(time);      \
    } else if constexpr (ScaleTo == TimeScale::UT1_SCALE) { \
        return UNITE_4(convert, SCALE_FROM, _, UT1)(time);      \
    } else if constexpr (ScaleTo == TimeScale::TAI_SCALE) { \
        return UNITE_4(convert, SCALE_FROM, _, TAI)(time);      \
    } else if constexpr (ScaleTo == TimeScale::TDB_SCALE) { \
        return UNITE_4(convert, SCALE_FROM, _, TDB)(time);      \
    } else if constexpr (ScaleTo == TimeScale::TCB_SCALE) { \
        return UNITE_4(convert, SCALE_FROM, _, TCB)(time);      \
    } else if constexpr (ScaleTo == TimeScale::TCG_SCALE) { \
        return UNITE_4(convert, SCALE_FROM, _, TCG)(time);      \
    } else {                                                     \
        static_assert(AlwaysFalse<ScaleTo>);                     \
    }

    template<typename Dut>
    template<TimeScale ScaleTo, TimeScale ScaleFrom>
    Time<ScaleTo> TimeConverter<Dut>::convert(
            const Time<ScaleFrom> &time) const {
        if constexpr (ScaleFrom == ScaleTo) {
            return time;
        } else if constexpr (ScaleFrom == TimeScale::TT_SCALE) {
            CREATE_ONE_SCALE(TT)
        } else if constexpr (ScaleFrom == TimeScale::UTC_SCALE) {
            CREATE_ONE_SCALE(UTC)
        } else if constexpr (ScaleFrom == TimeScale::UT1_SCALE) {
            CREATE_ONE_SCALE(UT1)
        } else if constexpr (ScaleFrom == TimeScale::TAI_SCALE) {
            CREATE_ONE_SCALE(TAI)
        } else if constexpr (ScaleFrom == TimeScale::TDB_SCALE) {
            CREATE_ONE_SCALE(TDB)
        } else if constexpr (ScaleFrom == TimeScale::TCB_SCALE) {
            CREATE_ONE_SCALE(TCB)
        } else if constexpr (ScaleFrom == TimeScale::TCG_SCALE) {
            CREATE_ONE_SCALE(TCG)
        } else {
            static_assert(AlwaysFalse<ScaleFrom>);
        }
    }

#undef UNITE_4
#undef CREATE_ONE_SCALE

}

#endif //BALLISTICS2023_TIMECONVERTER_H
