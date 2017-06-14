#ifndef MODEL_H
#define MODEL_H

#include "geometry.h"
#include "primitives.h"
#include <list>

using std::vector;
using std::list;

/* A classe model guarda os vértices, arestas e faces do objeto,
 * e realiza a triangulação a partir da lista que ele possui
 */
class Model
{
public:
    Model();
    //Recebe os pontos (vértices), a fronteira e restrições (edges)
    Model(vector<Vertex*> _vertices, vector<Edge*> _edges);
    //Aplica a triangulação através do avanço de fronteira
    void triangulate();
    vector<WEdge*> findCreateEdge(Vertex *start, Vertex *end, Loop* loop, vector<WEdge*> frontier, WEdge *newEdge);
private:
    vector<Vertex*> vertices;
    vector<Edge*> edges;
    vector<Face*> faces;

    unsigned int edgeCounter = 0;
    unsigned int faceCounter = 0;
};

#endif // MODEL_H
