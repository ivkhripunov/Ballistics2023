//
// Created by ivankhripunov on 19.10.23.
//

#ifndef BALLISTICS2023_BASICTYPES_H
#define BALLISTICS2023_BASICTYPES_H

#include "Eigen/Core"
#include "Eigen/Dense"
#include "Eigen/Geometry"
#include <vector>

namespace Ballistics {

    using scalar = double;
    using indexType = std::size_t;

    template<typename T>
    using Quaternion = Eigen::Quaternion<T>;
    template<typename T>
    using Vector3 = Eigen::Vector3<T>;
    template<typename T, indexType size>
    using Vector = Eigen::Vector<T, size>;
    template<typename T>
    using Matrix3 = Eigen::Matrix<T, 3, 3>;

    namespace Containers {

        template<typename RealType>
        using vector = std::vector<RealType>;

        template<typename RealType, indexType N>
        using array = std::array<RealType, N>;

        using string = std::string;
    }

    using Vector3d = Eigen::Vector3<double>;
    using Matrix3d = Eigen::Matrix<double, 3, 3>;

}

#endif //BALLISTICS2023_BASICTYPES_H
