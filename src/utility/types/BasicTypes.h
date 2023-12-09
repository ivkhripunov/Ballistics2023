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
    template<typename T>
    using Matrix3 = Eigen::Matrix<T, 3, 3>;

    namespace Containers {
        template<typename RealType>
        using vector = std::vector<RealType>;
        using string = std::string;
    }

    using scalar = double;
    using Vector3d = Eigen::Vector3<scalar>;

    using indexType = std::size_t;


    class Quaternion {
        scalar a_;
        scalar b_;
        scalar c_;
        scalar d_;

    public:
        Quaternion(const scalar a, const scalar b, const scalar c, const scalar d) : a_(a), b_(b), c_(c), d_(d) {}

        Quaternion(const double matrix[3][3]) {
            const double trace = matrix[0][0] + matrix[1][1] + matrix[2][2];

            if (trace > 0) {
                const double S = 0.5 / std::sqrt(trace + 1.0);
                const auto qw = static_cast<scalar>(0.25 / S);
                const auto qx = static_cast<scalar>((matrix[2][1] - matrix[1][2]) * S);
                const auto qy = static_cast<scalar>((matrix[0][2] - matrix[2][0]) * S);
                const auto qz = static_cast<scalar>((matrix[1][0] - matrix[0][1]) * S);
            } else {
                if (matrix[0][0] > matrix[1][1] && matrix[0][0] > matrix[2][2]) {
                    const double S = 2.0 * std::sqrt(1.0 + matrix[0][0] - matrix[1][1] - matrix[2][2]);
                    const auto qw = static_cast<scalar>((matrix[2][1] - matrix[1][2]) / S);
                    const auto qx = static_cast<scalar>(0.25 * S);
                    const auto qy = static_cast<scalar>((matrix[0][1] + matrix[1][0]) / S);
                    const auto qz = static_cast<scalar>((matrix[0][2] + matrix[2][0]) / S);
                } else if (matrix[1][1] > matrix[2][2]) {
                    double S = 2.0 * std::sqrt(1.0 + matrix[1][1] - matrix[0][0] - matrix[2][2]);
                    const auto qw = static_cast<scalar>((matrix[0][2] - matrix[2][0]) / S);
                    const auto qx = static_cast<scalar>((matrix[0][1] + matrix[1][0]) / S);
                    const auto qy = static_cast<scalar>(0.25 * S);
                    const auto qz = static_cast<scalar>((matrix[1][2] + matrix[2][1]) / S);
                } else {
                    const double S = 2.0 * std::sqrt(1.0 + matrix[2][2] - matrix[0][0] - matrix[1][1]);
                    const auto qw = static_cast<scalar>((matrix[1][0] - matrix[0][1]) / S);
                    const auto qx = static_cast<scalar>((matrix[0][2] + matrix[2][0]) / S);
                    const auto qy = static_cast<scalar>((matrix[1][2] + matrix[2][1]) / S);
                    const auto qz = static_cast<scalar>(0.25 * S);
                }
            }

//            a_ = qw;
//            b_ = qx;
//            c_ = qy;
//            d_ = qz;
        }
    };
}

#endif //BALLISTICS2023_BASICTYPES_H
