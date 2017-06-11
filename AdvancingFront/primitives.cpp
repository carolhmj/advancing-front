#include "primitives.h"


float Primitives::distance(glm::vec2 a, glm::vec2 b)
{
    return (a-b).length();
}


float Primitives::angle(glm::vec2 a, glm::vec2 b)
{
    return std::acos(glm::dot(a,b) / (a.length() * b.length()));
}
