//
// Created by Leo Gogichaishvili on 15.02.22.
//

#ifndef RAYCASTER_GAMEMATH_H
#define RAYCASTER_GAMEMATH_H

#include <SFML/Graphics.hpp>
#include "math.h"


namespace GosRender {

    float VectorLength(const sf::Vector2f&);

    float AngleBetweenVectors(const sf::Vector2f&, const sf::Vector2f&);

    float CosineOfAngleBetweenVectors(const sf::Vector2f&, const sf::Vector2f&);

    sf::Vector2f Normalize(const sf::Vector2f&);

    extern const float PI;
}


#endif //RAYCASTER_GAMEMATH_H
