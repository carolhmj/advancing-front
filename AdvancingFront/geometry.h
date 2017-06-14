#ifndef GEOMETRY
#define GEOMETRY

#include "glm/vec2.hpp"

struct Vertex {
    unsigned int id;
    glm::vec2 pos;

    Vertex(unsigned int _id, glm::vec2 _pos) : id(_id), pos(_pos) {  }
};

struct Edge {
    Vertex *a, *b;

    Edge(Vertex *_a, Vertex *_b) : a(_a), b(_b) { }

    double value() {
        return b->pos - a->pos;
    }
};

struct Face {
    Vertex *a, *b, *c;

    Face(Vertex *_a, Vertex *_b, Vertex *_c) : a(_a), b(_b), c(_c) {}
};

#endif // GEOMETRY

