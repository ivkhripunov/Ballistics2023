//
// Created by ivankhripunov on 08.02.24.
//

#include <gtest/gtest.h>

#include "time/Time.h"
#include "utility/numericalMethods/RungeKutta.h"
#include "utility/RHS/CentralGravity.h"

#include "fstream"

TEST(RK_CENTRAL_FIELD, ORBIT1) {

    const std::string currentFile = __FILE__;
    const std::string outputPath = "/home/ivankhripunov/CLionProjects/ballistics2023/output/RK";
    //const std::string outputPath = currentFile.substr(0, currentFile.size() - 52) + "output/Rk";

    std::ofstream myfile(outputPath, std::ios::trunc);

    const Ballistics::scalar gravParameter = 3.986e14;
    const Ballistics::Vector3d position = {6700000, 0, 0};
    const Ballistics::Vector3d velocity = {0, std::sqrt(gravParameter / position.norm()), 0};
    const Ballistics::TimeModule::Time<Ballistics::TimeModule::TimeScale::TT_SCALE> initialTimeTT(0, 0);
    const Ballistics::RHS::CentralGravity rhs;
    const Ballistics::RHS::CentralGravity::State initialState = {position, velocity, initialTimeTT};

    const Ballistics::scalar T = 2 * M_PI * position.norm() * std::sqrt(position.norm() / gravParameter);

    for (Ballistics::scalar step = T / 8; step >= T / 4096; step /= 2) {

        Ballistics::RHS::CentralGravity::State result = initialState;
        const Ballistics::NumericalMethods::IntegrationParameters integrationParameters = {step};

        for (Ballistics::indexType i = 0; i < T / step; ++i) {
            result = Ballistics::NumericalMethods::integrateOneStep<Ballistics::NumericalMethods::RK4>(rhs, result,
                                                                                                       integrationParameters);
            std::cout << result.position << std::endl;

        }
        std::cout << std::endl;

        myfile << step << " " << (result.position - initialState.position).norm() << std::endl;
    }

}
