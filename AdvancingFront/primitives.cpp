#include "primitives.h"


float Primitives::distance(glm::vec2 a, glm::vec2 b)
{
    return (a-b).length();
}


float Primitives::angle(glm::vec2 a, glm::vec2 b)
{
    double x = glm::dot(a,b) / (glm::length(a) * glm::length(b));
    return std::acos(x);
}


bool Primitives::isRightTo(glm::vec2 a, glm::vec2 b)
{
    float cross = a.x*b.y - a.y*b.x;
    return cross < 0;
}

bool Primitives::intersects(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d)
{
    bool x = cross2d(b-a, c-a) * cross2d(b-a, d-a) < 0;
    bool y = cross2d(d-c, a-c) * cross2d(d-c, b-c) < 0;
    return x && y;
}


float Primitives::cross2d(glm::vec2 a, glm::vec2 b)
{
    return a.x*b.y - b.x*a.y;
}
