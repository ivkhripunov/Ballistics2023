//
// Created by ivankhripunov on 19.10.23.
//

#ifndef BALLISTICS2023_VECTOR_H
#define BALLISTICS2023_VECTOR_H

#include "Eigen/Core"

namespace Ballistics {
    template<typename T>
    using Vector3 = Eigen::Vector3<T>;

    using scalar = double;
    using Vector3d = Eigen::Vector3<scalar>;

    using indexType = std::size_t;
}

#endif //BALLISTICS2023_VECTOR_H
