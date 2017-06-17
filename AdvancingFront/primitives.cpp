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
