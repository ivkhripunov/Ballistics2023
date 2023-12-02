//
// Created by ivankhripunov on 19.10.23.
//

#ifndef BALLISTICS2023_BASICTYPES_H
#define BALLISTICS2023_BASICTYPES_H

#include "Eigen/Core"
#include <vector>

namespace Ballistics {
    template<typename T>
    using Vector3 = Eigen::Vector3<T>;

    namespace Containers {
        template<typename RealType>
        using vector = std::vector<RealType>;
        using string = std::string;
    }

    using scalar = double;
    using Vector3d = Eigen::Vector3<scalar>;

    using indexType = std::size_t;
}

#endif //BALLISTICS2023_BASICTYPES_H
