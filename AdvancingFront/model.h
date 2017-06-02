#ifndef MODEL_H
#define MODEL_H

#include "geometry.h"

using namespace Geometry;
using std::vector;

/* A classe model guarda os vértices, arestas e faces do objeto,
 * e realiza a triangulação a partir da lista que ele possui
 */
class Model
{
public:
    Model();
    //Recebe os pontos (vértices), a fronteira e restrições (edges)
    Model(vector<Vertex> _vertices, vector<WEdge> _edges);
    //Aplica a triangulação através do avanço de fronteira
    void triangulate();
private:
    vector<Vertex> vertices;
    vector<WEdge> edges;
    vector<Loop> loops;
};

#endif // MODEL_H
