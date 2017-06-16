#include "model.h"
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

Model::Model()
{

}

Model::Model(vector<Vertex *> _vertices, vector<Edge *> _edges) : vertices(_vertices), edges(_edges) {
    edgeCounter = _edges.size() + 1;
    faceCounter = 1;
}

//TODO colocar iedge nos pontos internos
void Model::triangulate()
{
    //Iniciamos com a fronteira recebendo a borda e as restrições
    vector<Edge*> frontier = edges;

    //Enquanto a fronteira não está vazia
    while (!frontier.empty()) {
        std::cout << "Examining frontier:" << endl;
        for (Edge*& e : frontier) {
            cout << e->print() << endl;
        }
        //Pega a próxima aresta a ser analisada
        Edge *currEdge = frontier.front();

        cout << "curr edge: " << currEdge->print() << endl;

        Vertex *a = currEdge->a;
        Vertex *b = currEdge->b;

        //Encontra o próximo ponto
        Vertex *point = nullptr; // = find_point()

        float maxAngle = std::numeric_limits<float>::lowest();

        //Vamos utilizar o critério de delaunay (maior ângulo) com todos os pontos
        for (Vertex*& v : vertices) {
            if (v != a && v != b) {
                float angle = Primitives::angle(a->pos - v->pos, b->pos - v->pos);
                if (angle > maxAngle) {
                    point = v;
                    maxAngle = angle;
                }
            }
        }

        //Caso não encontremos um ponto, o modelo não é triangularizável
        if (point == nullptr) {
            std::cout << "Modelo não pode ser triangularizado!\n";
            return;
        }

        /*
         * Primeiro, vamos descobrir se já existe uma aresta dos extremos da aresta escolhida para o ponto
         * se a currEdge for a->b e o ponto for p, queremos b->p e p->a, se as arestas encontradas forem na
         * direção contrária, precisamos ter cuidado
         */

        //Criar o novo triângulo
        Face *newFace = new Face(currEdge->b, point, currEdge->a);
        faces.push_back(newFace);

        currEdge->visits++;

        if (currEdge->shouldRemove()) {
            cout << "remove a aresta " << currEdge->print() << "da fronteira\n";
            frontier.erase(std::remove_if(frontier.begin(), frontier.end(), [currEdge](Edge *ec) {return currEdge == ec;} ));
        }

        //Referências às arestas que serão encontradas/criadas
        Edge *bpEdge;
        Edge *paEdge;

        frontier = findCreateEdge(b, point, frontier, bpEdge);
        frontier = findCreateEdge(point, a, frontier, paEdge);

    }

    cout << "triangulate ended with faces " << endl;
    for (Face*& f : faces) {
        cout << f->print() << endl;
    }
}

vector<Edge*> Model::findCreateEdge(Vertex *start, Vertex *end, vector<Edge*> frontier, Edge *newEdge)
{
    vector<Edge*> adj = findAdjEdges(start, frontier);
    bool found = false;
    for (Edge*& e : adj) {
        //Caso na direção que queremos
        if (e->a == start && e->b == end) {
            cout << "Encontramos a aresta " << e->print() << endl;
            found = true;
            e->visits++;
            if (e->shouldRemove()) {
                frontier.erase(std::remove_if(frontier.begin(), frontier.end(), [e](Edge *ec) {return e == ec;} ));
            }
        }
        //Comentando aqui por enquanto, porque se estiver na direção oposta, vamos querer que crie uma aresta nova!
//        //Caso na direção oposta
//        else if (e->b == start && e->a == end) {
//            cout << "encontramos a aresta " << e->print() << endl;
//            found = true;
//            e->visits++;
//            if (e->shouldRemove()) {
//                frontier.erase(std::remove_if(frontier.begin(), frontier.end(), [e](Edge *ec) {return e == ec;} ));
//            }
//        }

    }
    //Não encontramos, então vamos criar a aresta
    if (!found) {
        newEdge = new Edge(start, end);
        edges.push_back(newEdge);
        cout << "criamos a aresta " << newEdge->print() << endl;

        newEdge->type = EType::INTERNAL;
        newEdge->visits = 1;

        //Não vou botar na fronteira pq já visitei 1 vez!

//        frontier.push_back(newEdge);
    }

    return frontier;
}

//Procura as arestas adjacentes no vetor v
vector<Edge *> Model::findAdjEdges(Vertex *v, vector<Edge *> edges)
{
    vector<Edge*> founds;

    for (Edge*& e : edges) {
        if (e->a == v || e->b == v) {
            founds.push_back(e);
        }
    }

    return founds;
}

