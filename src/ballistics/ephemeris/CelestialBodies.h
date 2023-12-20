//
// Created by ivankhripunov on 17.12.23.
//

#ifndef BALLISTICS2023_CELESTIALBODIES_H
#define BALLISTICS2023_CELESTIALBODIES_H

namespace Ballistics {

// Порядок обусловлен библиотекой calceph
// https://www.imcce.fr/content/medias/recherche/equipes/asd/calceph/calceph_c.pdf стр. 19
    enum class CelestialBodiesEnum {
        MERCURY = 1,
        VENUS = 2,
        EARTH = 3,
        MARS = 4,
        JUPITER = 5,
        SATURN = 6,
        URANUS = 7,
        NEPTUNE = 8,
        PLUTO = 9,
        MOON = 10,
        SUN = 11,
        SOLAR_SYSTEM_BARYCENTER = 12,
        EARTH_MOON_BARYCENTER = 13
    };
}



#endif //BALLISTICS2023_CELESTIALBODIES_H
