//
// Created by ivankhripunov on 08.02.24.
//

#include <gtest/gtest.h>

#include "time/Time.h"
#include "utility/numericalMethods/RungeKutta.h"
#include "utility/RHS/CentralGravity.h"
#include "utility/RHS/EarthGravity.h"
#include "utility/RHS/RV.h"

#include "utility/parser/BulletinParser.h"

#include "time/DutCorrection.h"
#include "time/TimeConverter.h"

#include "frame/PolarMotionContainer.h"
#include "frame/FrameConverter.h"

#include "ephemeris/EphemerisCalculator.h"

#include "forces/Gravity/CelestialGravity.h"
#include "forces/Gravity/EarthGravity.h"

#include "atmosphere/GOST4401_81.h"
#include "forces/AtmosphereDrag/AtmosphereDrag.h"

#include "forces/AccelerationCalculator.h"

#include "fstream"

//TEST(RK_CENTRAL_FIELD, ORBIT1) {
//
//    const std::string currentFile = __FILE__;
//    const std::string outputPath = "/home/ivankhripunov/CLionProjects/ballistics2023/output/RK";
//    //const std::string outputPath = currentFile.substr(0, currentFile.size() - 52) + "output/Rk";
//
//    std::ofstream myfile(outputPath, std::ios::trunc);
//
//    const Ballistics::scalar gravParameter = 3.986e14;
//    const Ballistics::Vector3d position = {initialRho, 0, 0};
//    const Ballistics::Vector3d velocity = {0, std::sqrt(gravParameter / position.norm()), 0};
//    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> initialTimeTT(0, 0);
//    const Ballistics::RHS::CentralGravity rhs;
//    const Ballistics::RHS::CentralGravity::State initialState = {position, velocity, initialTimeTT};
//
//    const Ballistics::scalar T = 2 * M_PI * position.norm() * std::sqrt(position.norm() / gravParameter);
//
//    for (Ballistics::scalar step = T / 8; step >= T / 4096; step /= 2) {
//
//        Ballistics::RHS::CentralGravity::State result = initialState;
//        const Ballistics::NumericalMethods::IntegrationParameters integrationParameters = {step};
//
//        for (Ballistics::indexType i = 0; i < static_cast<Ballistics::indexType>(T / step); ++i) {
//            result = Ballistics::NumericalMethods::integrateOneStep<Ballistics::NumericalMethods::RK4>(rhs, result,
//                                                                                                       integrationParameters);
//
//        }
//
//        myfile << step << " " << (result.position - initialState.position).norm() << std::endl;
//    }
//
//}
//
//TEST(RK_CENTRAL_FIELD_INTERVAL, ORBIT1) {
//
//    const std::string currentFile = __FILE__;
//    const std::string outputPath = "/home/ivankhripunov/CLionProjects/ballistics2023/output/RK_INTERVAL";
//    //const std::string outputPath = currentFile.substr(0, currentFile.size() - 52) + "output/Rk";
//
//    std::ofstream myfile(outputPath, std::ios::trunc);
//
//    const Ballistics::scalar gravParameter = 3.986e14;
//    const Ballistics::Vector3d position = {initialRho, 0, 0};
//    const Ballistics::Vector3d velocity = {0, std::sqrt(gravParameter / position.norm()), 0};
//    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> initialTimeTT(0, 0);
//    const Ballistics::RHS::CentralGravity rhs;
//    const Ballistics::RHS::CentralGravity::State initialState = {position, velocity, initialTimeTT};
//
//    const Ballistics::scalar T = 2 * M_PI * initialRho * std::sqrt(initialRho / gravParameter);
//
//    for (Ballistics::scalar step = T / 8; step >= T / 4096; step /= 2) {
//
//        const auto endTimeTT = Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE>::buildFromJD(
//                T / 86400);
//
//        Ballistics::RHS::CentralGravity::State result = Ballistics::NumericalMethods::integrate<Ballistics::NumericalMethods::RK4>(
//                rhs, initialState, endTimeTT, step);
//
//        myfile << step << " " << (result.position - initialState.position).norm() << std::endl;
//
//        //std::cout << step << std::endl;
//    }
//
//}
//
//TEST(RK_EARTH_GRAVITY, ORBIT1) {
//
//    const std::string currentFile = __FILE__;
//    const std::string outputPath = "/home/ivankhripunov/CLionProjects/ballistics2023/output/RK_INTERVAL";
//    //const std::string outputPath = currentFile.substr(0, currentFile.size() - 52) + "output/Rk";
//    const std::string modelPath = currentFile.substr(0, currentFile.size() - 56) + "data/earthGravity";
//
//    std::ofstream myfile(outputPath, std::ios::trunc);
//
//    const Ballistics::scalar gravParameter = 3.986e14;
//    const Ballistics::Vector3d position = {initialRho, 0, 0};
//    const Ballistics::Vector3d velocity = {0, std::sqrt(gravParameter / position.norm()), 0};
//    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> initialTimeTT(0, 0);
//    const Ballistics::RHS::EarthGravity rhs(modelPath, "egm96", 64, 64);
//    const Ballistics::RHS::EarthGravity::State initialState = {position, velocity, initialTimeTT};
//
//    const Ballistics::scalar T = 2 * M_PI * initialRho * std::sqrt(initialRho / gravParameter);
//
//    for (Ballistics::scalar step = T / 8; step >= T / 4096; step /= 2) {
//
//        const auto endTimeTT = Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE>::buildFromJD(
//                T / 86400);
//
//        Ballistics::RHS::EarthGravity::State result = Ballistics::NumericalMethods::integrate<Ballistics::NumericalMethods::RK4>(
//                rhs, initialState, endTimeTT, step);
//
//        myfile << step << " " << (result.position - initialState.position).norm() << std::endl;
//
//        //std::cout << step << std::endl;
//    }
//
//}

TEST(RK_FORCE_CALCULATOR, ORBIT1) {

    const std::string currentFile = __FILE__;

    const auto begin = static_cast<Ballistics::scalar>(0);
    const auto end = static_cast<Ballistics::scalar>(100000);

    const std::string csvPath = currentFile.substr(0, currentFile.size() - 56) + "data/frame/earth_rotation.csv";

    const Ballistics::indexType mjdColumnIndex = 3;
    const Ballistics::indexType xpColumnIndex = 4;
    const Ballistics::indexType ypColumnIndex = 5;
    const Ballistics::indexType dutColumnIndex = 6;

    Ballistics::Containers::vector<double> mjdData = Ballistics::Utility::getColumn(csvPath, mjdColumnIndex,
                                                                                    begin, end);

    Ballistics::Containers::vector<double> xpData = Ballistics::Utility::getColumn(csvPath, xpColumnIndex,
                                                                                   begin, end);

    Ballistics::Containers::vector<double> ypData = Ballistics::Utility::getColumn(csvPath, ypColumnIndex,
                                                                                   begin, end);

    Ballistics::Containers::vector<double> dutData = Ballistics::Utility::getColumn(csvPath, dutColumnIndex,
                                                                                    begin, end);

    const Ballistics::FrameModule::PolarMotionContainer polarMotionContainer(mjdData, xpData, ypData);
    const Ballistics::TimeModule::DutContainer dutContainer(mjdData, dutData);


    const Ballistics::TimeModule::TimeConverter<Ballistics::TimeModule::DutContainer> timeConverter(dutContainer);


    const Ballistics::FrameModule::FrameConverter<Ballistics::FrameModule::PolarMotionContainer,
            Ballistics::TimeModule::DutContainer> frameConverter(
            polarMotionContainer, timeConverter);


    const std::string ephemerisPath = currentFile.substr(0, currentFile.size() - 56) + "data/ephemeris/de405.bin";
    //const Ballistics::Containers::string ephemerisPath = "/home/ivankhripunov/CLionProjects/ballistics2023/data/ephemeris/de405.bin";
    const Ballistics::Ephemeris::EphemerisCalculator ephemerisCalculator(ephemerisPath);


    const std::string earthPath = currentFile.substr(0, currentFile.size() - 56) + "data/earthGravity";
    Ballistics::Force::EarthGravityForce earthGravity(earthPath,
                                                      "egm96", 4, 4);

    Ballistics::Containers::vector<Ballistics::Utility::Interpolator<Ballistics::scalar, Ballistics::scalar>::XY> xy(
            Ballistics::Atmosphere::GOST4401_81Raw::N);

    for (Ballistics::indexType i = 0; i < Ballistics::Atmosphere::GOST4401_81Raw::N; ++i) {
        xy[i] = {Ballistics::Atmosphere::GOST4401_81Raw::height[i],
                 Ballistics::Atmosphere::GOST4401_81Raw::density[i]};
    }

    const Ballistics::Utility::Interpolator<Ballistics::scalar, Ballistics::scalar> interpolator(xy);

    const Ballistics::Atmosphere::DensityCalculator densityCalculator(interpolator);

    const Ballistics::Force::AtmosphereDrag atmosphereDrag(densityCalculator);

    /******************************************************/

    auto forcesTuple = std::tuple(atmosphereDrag);

    const double mass = 1;

    const Ballistics::Force::AccelerationCalculator accelerationCalculator(timeConverter, frameConverter,
                                                                           ephemerisCalculator, earthGravity,
                                                                           forcesTuple);

    decltype(accelerationCalculator)::allSatParams allSatParams;
    allSatParams.dragCoeff_ = 1;
    allSatParams.dragArea_ = 0.001;

    const Ballistics::RHS::RV RVcalculator(accelerationCalculator);

    const Ballistics::scalar gravParameter = 3.986e14;
    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> timeTT(2452793.5, 0);
    const Ballistics::scalar initialRho = 6.7e6;
    const Ballistics::Vector3d position = {initialRho, 0, 0};
    const Ballistics::Vector3d velocity = {0, std::sqrt(gravParameter / position.norm()), 0};
    const decltype(RVcalculator)::State initialState = {position, velocity, timeTT};

    const Ballistics::scalar T = 2 * std::numbers::pi * initialRho * std::sqrt(initialRho / gravParameter);

    const auto step = 1;

    const auto endTimeTT = timeTT + 2 * T;

    const auto result = Ballistics::NumericalMethods::integrate<Ballistics::NumericalMethods::RK4>(
            RVcalculator, initialState, endTimeTT, step, allSatParams, mass);

    std::cout << result.back().position.norm() - initialRho;
}