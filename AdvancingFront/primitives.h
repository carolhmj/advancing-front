#ifndef PRIMITIVES
#define PRIMITIVES

#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include <cmath>

namespace Primitives {
    //Distância entre pontos
    float distance(glm::vec2 a, glm::vec2 b);
    float angle(glm::vec2 a, glm::vec2 b);
    bool isRightTo(glm::vec2 a, glm::vec2 b);
    float cross2d(glm::vec2 a, glm::vec2 b);
    //Interseção dos segmentos AB e CD
    bool intersects(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d);
}

#endif // PRIMITIVES
