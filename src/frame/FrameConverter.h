//
// Created by ivankhripunov on 09.12.23.
//

#ifndef BALLISTICS2023_FRAMECONVERTER_H
#define BALLISTICS2023_FRAMECONVERTER_H

#include "PolarMotionContainer.h"

namespace Ballistics::FrameModule {

    template<typename PolarMotionContainer, typename DutContainer>
    class FrameConverter {
        PolarMotionContainer polarMotionContainer;
        TimeModule::TimeConverter<DutContainer> timeConverter;

    public:
        /**
         *  Алгоритм 5.6 sofa earth attitude
         * @param tt время по шкале TT
         * @return кватернион перехода от GCRS в ITRS
         */
        [[nodiscard]] Quaternion convertGCRStoITRS(const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> &tt);

    };
}

#endif //BALLISTICS2023_FRAMECONVERTER_H
