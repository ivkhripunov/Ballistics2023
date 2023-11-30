//
// Created by ivankhripunov on 30.11.23.
//

#ifndef BALLISTICS2023_TIMECONVERTER_H
#define BALLISTICS2023_TIMECONVERTER_H

#include "Time.h"

// Вопросы
// 1. Нужен ли constexpr?

namespace Ballistics::TimeModule {


    template<typename RealType, typename DutContainer>
    class TimeConverter {
        DutContainer dutContainer_;

    public:
        /**
         * Cоздание конвертера через дут контейнер
         */
        TimeConverter(const DutContainer &dutContainer) noexcept;

        template<Ballistics::TimeModule::TimeScale To, Ballistics::TimeModule::TimeScale From>
        Time<RealType, To> convert(const Time<RealType, From> &from) const;

        [[nodiscard]] Time<RealType, Ballistics::TimeModule::TimeScale::UTC_SCALE>
        convertTAI_UTC(const Time<RealType, Ballistics::TimeModule::TimeScale::TAI_SCALE> &tai) const noexcept;
    };


    template<typename RealType, typename DutContainer>
    TimeConverter<RealType, DutContainer>::TimeConverter(const DutContainer &dutContainer) noexcept : dutContainer_(
            dutContainer) {}

    //TODO: add all conversions
    template<typename RealType, typename DutContainer>
    template<Ballistics::TimeModule::TimeScale To, Ballistics::TimeModule::TimeScale From>
    Time<RealType, To> TimeConverter<RealType, DutContainer>::convert(const Time<RealType, From> &from) const {

        if (To == TimeScale::UTC_SCALE && From == TimeScale::TAI_SCALE) return convertTAI_UTC(from);
    }

    template<typename RealType, typename DutContainer>
    Time<RealType, Ballistics::TimeModule::TimeScale::UTC_SCALE> TimeConverter<RealType, DutContainer>::convertTAI_UTC(
            const Time<RealType, Ballistics::TimeModule::TimeScale::TAI_SCALE> &tai) const noexcept {

        RealType jdIntTAI, jdFracTAI;

        const int status = iauTaiutc(tai.jdDayInt(), tai.jdDayFrac(), &jdIntTAI, &jdFracTAI);

        switch (status) {
            case 1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: DUBIOUS YEAR");

            case -1:
                throw Ballistics::Exceptions::TimeModuleException("SOFA FAILED: UNACCEPTABLE DATE");

            default:
                return Time<RealType, Ballistics::TimeModule::TimeScale::UTC_SCALE>(jdIntTAI, jdFracTAI);
        }
    }
}

#endif //BALLISTICS2023_TIMECONVERTER_H
