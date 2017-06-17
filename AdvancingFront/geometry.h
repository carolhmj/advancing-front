#ifndef GEOMETRY
#define GEOMETRY

#include "glm/vec2.hpp"
#include <string>
#include <sstream>
#include "glm/gtx/string_cast.hpp"

enum EType {
    BORDER,
    INTERNAL
};

struct Vertex {
    unsigned int id;
    glm::vec2 pos;

    Vertex(unsigned int _id, glm::vec2 _pos) : id(_id), pos(_pos) {  }

    std::string print() {
        std::stringstream ss;
        ss << "{" << id << ": " << glm::to_string(pos) << "}";
        return ss.str();
    }
};

struct Edge {
    Vertex *a, *b;
    unsigned int visits = 0;
    EType type;

    Edge(Vertex *_a, Vertex *_b) : a(_a), b(_b) { }

    glm::vec2 value() {
        return b->pos - a->pos;
    }

    bool shouldRemove() {
        return (visits >= 1 && type == EType::BORDER) || (visits >= 1 && type == EType::INTERNAL);
    }

    std::string print() {
        std::stringstream ss;
        ss << "[ " << a->id << " " << b->id << " ]";
        return ss.str();
    }
};

struct Face {
    Vertex *a, *b, *c;

    Face(Vertex *_a, Vertex *_b, Vertex *_c) : a(_a), b(_b), c(_c) {}

    std::string print() {
        std::stringstream ss;
        ss << "< " << a->id << " " << b->id << " " << c->id << ">";
        return ss.str();
    }
};

#endif // GEOMETRY

