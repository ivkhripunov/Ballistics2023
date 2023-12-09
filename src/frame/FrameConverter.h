//
// Created by ivankhripunov on 09.12.23.
//

#ifndef BALLISTICS2023_FRAMECONVERTER_H
#define BALLISTICS2023_FRAMECONVERTER_H

// Нужны ли коррекции?

#include "utility/constants/MathConstants.h"
#include "PolarMotionContainer.h"

namespace Ballistics::FrameModule {

    template<typename PolarMotionContainer, typename DutContainer>
    class FrameConverter {
        PolarMotionContainer polarMotionContainer_;
        TimeModule::TimeConverter<DutContainer> timeConverter_;

    public:

        FrameConverter(const PolarMotionContainer &polarMotionContainer,
                       const TimeModule::TimeConverter<DutContainer> &timeConverter) :
                polarMotionContainer_(polarMotionContainer), timeConverter_(timeConverter) {}

        /**
         *  Алгоритм 5.6 sofa earth attitude
         * @param tt время по шкале TT
         * @return матрицу перехода от GCRS в ITRS
         */
        [[nodiscard]] Matrix3<scalar>
        conversionMatrixGCRStoITRS(const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> &tt) const;
    };

    template<typename PolarMotionContainer, typename DutContainer>
    Matrix3<scalar> FrameConverter<PolarMotionContainer, DutContainer>::conversionMatrixGCRStoITRS(
            const TimeModule::Time<TimeModule::TimeScale::TT_SCALE> &tt) const {

        double cipX, cipY; // X, Y coordinates of celestial intermediate pole from series based on IAU 2006 precession and IAU 2000A nutation.

        auto ttInt = static_cast<double>(tt.jdDayInt());
        auto ttFrac = static_cast<double>(tt.jdDayFrac());

        /* CIP and CIO, IAU 2006/2000A. */
        iauXy06(ttInt, ttFrac, &cipX, &cipY);
        const double cioLocator = iauS06(ttInt, ttFrac, cipX, cipY); // в радианах, aka "s"

        /* GCRS to CIRS matrix. */
        double rc2i[3][3];
        // Form the celestial to intermediate-frame-of-date matrix given the CIP X,Y and the CIO locator s.
        iauC2ixys(cipX, cipY, cioLocator, rc2i);

        const TimeModule::Time<TimeModule::TimeScale::UT1_SCALE> ut1 = timeConverter_.convertTT_UT1(
                tt);

        auto ut1Int = static_cast<double>(ut1.jdDayInt());
        auto ut1Frac = static_cast<double>(ut1.jdDayFrac());

        /* Earth rotation angle. */
        const double era = iauEra00(ut1Int, ut1Frac);

        /*Form celestial-terrestrial matrix (no polar motion yet). */
        double rc2ti[3][3];
        iauCr(rc2i, rc2ti);
        iauRz(era, rc2ti);

        /* Polar motion matrix (TIRS->ITRS, IERS 2003). */
        const TimeModule::Time<TimeModule::TimeScale::UTC_SCALE> utc = timeConverter_.convertUT1_UTC(
                ut1);
        const PolarMotion polarMotion = polarMotionContainer_.getPolarMotion(utc);
        const auto xp = static_cast<double>(polarMotion.xp_ * arcsecToRadians);
        const auto yp = static_cast<double>(polarMotion.yp_ * arcsecToRadians);
        const double sp = iauSp00(ttInt, ttFrac);
        double rpom[3][3];
        iauPom00(xp, yp, sp, rpom);

        double rc2it[3][3];
        /* Form celestial-terrestrial matrix (including polar motion). */
        iauRxr(rpom, rc2ti, rc2it);

        Eigen::Matrix3<scalar> resultMatrix;

        resultMatrix
                <<
                static_cast<scalar>(rc2it[0][0]),
                static_cast<scalar>(rc2it[0][1]),
                static_cast<scalar>(rc2it[0][2]),
                static_cast<scalar>(rc2it[1][0]),
                static_cast<scalar>(rc2it[1][1]),
                static_cast<scalar>(rc2it[1][2]),
                static_cast<scalar>(rc2it[2][0]),
                static_cast<scalar>(rc2it[2][1]),
                static_cast<scalar>(rc2it[2][2]);

        return resultMatrix;
    }
}

#endif //BALLISTICS2023_FRAMECONVERTER_H
