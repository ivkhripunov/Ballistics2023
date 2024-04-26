//
// Created by ivankhripunov on 19.04.24.
//

#ifndef BALLISTICS2023_GAUSSNOISE_H
#define BALLISTICS2023_GAUSSNOISE_H

#include <random>

#include "utility/types/BasicTypes.h"

namespace Ballistics {

    std::vector<Vector<scalar, 6>>
    noiseRVStates(const std::vector<Vector<scalar, 6>> &measurements, const Ballistics::scalar sigmaX,
                  const Ballistics::scalar sigmaVx) {
        /// Настраиваем генератор случайных чисел
            std::random_device rd;
            std::default_random_engine generator(rd());
        //std::default_random_engine generator;
        std::normal_distribution<Ballistics::scalar> distrX(0., sigmaX);
        std::normal_distribution<Ballistics::scalar> distrVx(0., sigmaVx);

        /// Теперь зашумляем результаты интегрирования
        std::vector<Vector<scalar, 6>> statesWithNoise(measurements);
        for (auto &vector: statesWithNoise) {
            for (int j = 0; j < 3; j++) {
                vector(j) += distrX(generator);
            }
            for (int j = 3; j < 6; j++) {
                vector(j) += distrVx(generator);
            }
        }

        return statesWithNoise;
    }

    Vector<scalar, 6>
    noiseRVState(const Vector<scalar, 6> &measurement, const Ballistics::scalar sigmaX,
                 const Ballistics::scalar sigmaVx) {
        /// Настраиваем генератор случайных чисел
            std::random_device rd;
            std::default_random_engine generator(rd());
        //std::default_random_engine generator;
        std::normal_distribution<Ballistics::scalar> distrX(0., sigmaX);
        std::normal_distribution<Ballistics::scalar> distrVx(0., sigmaVx);

        /// Теперь зашумляем результаты интегрирования
        Vector<scalar, 6> stateWithNoise = measurement;
        for (int j = 0; j < 3; j++) {
            stateWithNoise(j) += distrX(generator);
        }
        for (int j = 3; j < 6; j++) {
            stateWithNoise(j) += distrVx(generator);
        }


        return stateWithNoise;
    }

}

#endif //BALLISTICS2023_GAUSSNOISE_H
