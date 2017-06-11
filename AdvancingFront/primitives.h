#ifndef PRIMITIVES
#define PRIMITIVES

#include "glm/vec2.hpp"
#include "glm/geometric.hpp"

namespace Primitives {
    //Distância entre pontos
    float distance(glm::vec2 a, glm::vec2 b);
    float angle(glm::vec2 a, glm::vec2 b);
}

#endif // PRIMITIVES
