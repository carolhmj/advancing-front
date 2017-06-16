#ifndef MODEL_H
#define MODEL_H

#include "geometry.h"
#include "primitives.h"
#include <list>
#include <vector>

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
    vector<Edge*> findCreateEdge(Vertex *start, Vertex *end, vector<Edge*> frontier, Edge *newEdge);
    vector<Edge*> findAdjEdges(Vertex *v, vector<Edge*> edges);
private:
    vector<Vertex*> vertices;
    vector<Edge*> edges;
    vector<Face*> faces;

    unsigned int edgeCounter = 0;
    unsigned int faceCounter = 0;
};

#endif // MODEL_H
