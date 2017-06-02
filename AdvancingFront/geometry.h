#ifndef GEOMETRY
#define GEOMETRY

#include "glm/vec3.hpp"
#include <vector>

namespace Geometry {

struct WEdge;
struct Loop;

struct Vertex {
    //Identifier
    size_t id;
    //Position
    glm::vec3 position;
    //Incident edge
    WEdge *iedge;
    Vertex(size_t _id, glm::vec3 _position, WEdge* _iedge);

    //Returns the list of adjacent edges
    std::vector<WEdge*> adjedge();
    //Returns the list of adjacent vertices
    std::vector<Vertex*> adjvertex();
    //Returns the list of adjacent faces
    std::vector<Loop*> adjloop();

    // void draw();
};

struct Loop {
    //Identifier
    size_t id;
    //Incident edge
    WEdge *iedge;

    Loop(size_t _id, WEdge *_iedge);

    //Returns the list of adjacent edges
    std::vector<WEdge*> adjedge();
    //Returns the list of adjacent vertices
    std::vector<Vertex*> adjvertex();
    //Returns the list of adjacent faces
    std::vector<Loop*> adjloop();

    // void draw();
};

enum WEdgeType {
    BORDER,
    RESTRICTION,
    INTERNAL
};

struct WEdge {
    //Identifier
    size_t id;
    //Start and end vertices
    Vertex *vstart, *vend;
    //Clockwise and Counterclockwise faces
    Loop *cwloop, *ccwloop;
    //Clockwise predecessor and successor edges
    WEdge *cwpred, *cwsucc;
    //Counterclockwise predecessor and successor edges
    WEdge *ccwpred, *ccwsucc;
    //É borda, restrição ou interno?
    WEdgeType type;
    //Número de vezes que foi visitado
    unsigned int visits = 0;
    //Checa se ele deve ser removido
    bool shouldRemove();

    //Initialize only with id, fill other info later
    WEdge(size_t _id);
    //Initialize with all info
    WEdge(size_t id,
          Vertex* vstart, Vertex* vend,
          Loop* cwloop, Loop* ccwloop,
          WEdge* cwpred, WEdge* cwsucc,
          WEdge* ccwpred, WEdge* ccwsucc);

    //Returns the list of adjacent edges
    std::vector<WEdge*> adjedge();
    //Returns the list of adjacent vertices
    std::vector<Vertex*> adjvertex();
    //Returns the list of adjacent faces
    std::vector<Loop*> adjloop();

    // void draw();
};
}

#endif // GEOMETRY
