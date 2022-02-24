//
// Created by Leo Gogichaishvili on 15.02.22.
//

#include "GameMath.h"


float GosRender::VectorLength(const sf::Vector2f &vec) {
    return sqrt(pow(vec.x, 2.f) + pow(vec.y, 2.f));
}

float GosRender::AngleBetweenVectors(const sf::Vector2f &vec1, const sf::Vector2f &vec2) {
    float cosineOfAngle = ((vec1.x * vec2.x) + (vec1.y * vec2.y)) / (VectorLength(vec1) * VectorLength(vec2));
    return acos(cosineOfAngle);
}

float GosRender::CosineOfAngleBetweenVectors(const sf::Vector2f &vec1, const sf::Vector2f &vec2) {
    return ((vec1.x * vec2.x) + (vec1.y * vec2.y)) / (VectorLength(vec1) * VectorLength(vec2));
}

sf::Vector2f GosRender::Normalize(const sf::Vector2f &vec) {
    float vecLen = VectorLength(vec);
    return sf::Vector2f(vec.x / vecLen, vec.y / vecLen);
}


float GosRender::RadianToGradus(const float &radian) {
    return radian * (180 / PI);
}

const float GosRender::PI = 3.14159f;