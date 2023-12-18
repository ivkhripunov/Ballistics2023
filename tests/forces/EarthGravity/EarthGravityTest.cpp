//
// Created by ivankhripunov on 19.10.23.
//

#include "fstream"
#include <gtest/gtest.h>
#include "utility/types/BasicTypes.h"
#include "forces/Gravity/EarthGravity.h"

TEST(CALCFORCES, TEST) {

    std::ofstream myfile("/home/ivankhripunov/CLionProjects/ballistics2023/output/garmonic", std::ios::trunc);

    Ballistics::Force::EarthGravityForce earthGravity1x1("/home/ivankhripunov/CLionProjects/ballistics2023/data/",
                                                         "egm96", 1, 1);

    Ballistics::Force::EarthGravityForce earthGravity2x2("/home/ivankhripunov/CLionProjects/ballistics2023/data/",
                                                         "egm96", 2, 2);

    Ballistics::Force::EarthGravityForce earthGravity3x3("/home/ivankhripunov/CLionProjects/ballistics2023/data/",
                                                         "egm96", 3, 3);

    Ballistics::Force::EarthGravityForce earthGravity128x128("/home/ivankhripunov/CLionProjects/ballistics2023/data/",
                                                             "egm96", 128, 128);

    Ballistics::Force::EarthGravityForce earthGravity64x64("/home/ivankhripunov/CLionProjects/ballistics2023/data/",
                                                           "egm96", 64, 64);

    Ballistics::Force::EarthGravityForce earthGravity32x32("/home/ivankhripunov/CLionProjects/ballistics2023/data/",
                                                           "egm96", 32, 32);

    Ballistics::Force::EarthGravityForce earthGravity16x16("/home/ivankhripunov/CLionProjects/ballistics2023/data/",
                                                           "egm96", 16, 16);

    Ballistics::Force::EarthGravityForce earthGravity4x4("/home/ivankhripunov/CLionProjects/ballistics2023/data/",
                                                         "egm96", 4, 4);

    const double mass = 1;

    for (int i = 0; i < 177; i++) {

        std::cout << i << std::endl;

        double R = (200 * i + 6600) * 1e3;

        Ballistics::scalar meanDiffByR1122 = 0;
        Ballistics::scalar meanDiffByR2233 = 0;

        Ballistics::scalar meanDiffByR128x64 = 0;
        Ballistics::scalar meanDiffByR128x32 = 0;
        Ballistics::scalar meanDiffByR128x16 = 0;
        Ballistics::scalar meanDiffByR128x4 = 0;
        for (int m = 0; m < 100; m++) {
            for (int n = 0; n < 100; n++) {

                double Tet = M_1_PI / 100 * m;
                double Phi = 2 * M_1_PI / 100 * n;

                double x = R * std::sin(Tet) * std::cos(Phi);
                double y = R * std::sin(Tet) * std::sin(Phi);
                double z = R * std::cos(Tet);

                const Ballistics::Vector3d acceleration1x1 = earthGravity1x1.calcAcceleration({x, y, z});
                const Ballistics::Vector3d acceleration2x2 = earthGravity2x2.calcAcceleration({x, y, z});
                const Ballistics::Vector3d acceleration3x3 = earthGravity3x3.calcAcceleration({x, y, z});

                const Ballistics::Vector3d acceleration128x128 = earthGravity128x128.calcAcceleration({x, y, z});
                const Ballistics::Vector3d acceleration64x64 = earthGravity64x64.calcAcceleration({x, y, z});
                const Ballistics::Vector3d acceleration32x32 = earthGravity32x32.calcAcceleration({x, y, z});
                const Ballistics::Vector3d acceleration16x16 = earthGravity16x16.calcAcceleration({x, y, z});
                const Ballistics::Vector3d acceleration4x4 = earthGravity4x4.calcAcceleration({x, y, z});

                meanDiffByR1122 += (acceleration1x1 - acceleration2x2).norm();
                meanDiffByR2233 += (acceleration3x3 - acceleration2x2).norm();

                meanDiffByR128x64 += (acceleration128x128 - acceleration64x64).norm();
                meanDiffByR128x32 += (acceleration128x128 - acceleration32x32).norm();
                meanDiffByR128x16 += (acceleration128x128 - acceleration16x16).norm();
                meanDiffByR128x4 += (acceleration128x128 - acceleration4x4).norm();

//                myfile << R << " " << x << " " << y << " " << z << " " << acceleration.x() << " " << acceleration.y() << " "
//                       << acceleration.z() << std::endl;
            }
        }

        myfile << " " << R
               << " " << meanDiffByR1122 / 10000
               << " " << meanDiffByR2233 / 10000
               << " " << meanDiffByR128x64 / 10000
               << " " << meanDiffByR128x32 / 10000
               << " " << meanDiffByR128x16 / 10000
               << " " << meanDiffByR128x4 / 10000 << std::endl;
    }

}
