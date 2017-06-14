#include "model.h"
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

Model::Model()
{

}

Model::Model(vector<Vertex *> _vertices, vector<WEdge *> _edges) : vertices(_vertices), edges(_edges) { }

//TODO colocar iedge nos pontos internos
void Model::triangulate()
{
    //Iniciamos com a fronteira recebendo a borda e as restrições
    vector<WEdge*> frontier = edges;

    //Enquanto a fronteira não está vazia
    while (!frontier.empty()) {
        //Pega a próxima aresta a ser analisada
        WEdge *currEdge = frontier.front();

        cout << "curr edge: " << currEdge->id << endl;

        Vertex *a = currEdge->vstart;
        Vertex *b = currEdge->vend;

        cout << "a: " << a->id << " b: " << b->id << endl;

        //Encontra o próximo ponto
        Vertex *point = nullptr; // = find_point()

        float maxAngle = std::numeric_limits<float>::lowest();

        //Vamos utilizar o critério de delaunay (maior ângulo) com todos os pontos
        for (Vertex*& v : vertices) {
            if (v != a && v != b) {
                cout << "examining vertex: " << v->id << endl;
                float angle = Primitives::angle(a->position - v->position, b->position - v->position);
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
        Loop *newLoop = new Loop(loopCounter++, currEdge);
        loops.push_back(newLoop);

        cout << "criou o loop " << newLoop->id << endl;

        currEdge->ccwloop = newLoop;
        currEdge->visits++;
        if (currEdge->shouldRemove()) {
            cout << "remove a aresta " << currEdge->id << "da fronteira\n";
            frontier.erase(std::remove_if(frontier.begin(), frontier.end(), [currEdge](WEdge *ec) {return currEdge == ec;} ));
        }

        frontier = findCreateEdge(b, point, newLoop, frontier);
        frontier = findCreateEdge(point, a, newLoop, frontier);
    }
}

vector<WEdge*> Model::findCreateEdge(Vertex *start, Vertex *end, Loop *loop, vector<WEdge*> frontier)
{
    vector<WEdge*> adj = start->adjedge();
    bool found = false;
    for (WEdge*& e : adj) {
        //Caso na direção que queremos
        if (e->vstart == start && e->vend == end) {
            cout << "Encontramos a aresta " << e->id << endl;
            e->ccwloop = loop;
            loop->iedge = e;
            found = true;
            e->visits++;
            if (e->shouldRemove()) {
                frontier.erase(std::remove_if(frontier.begin(), frontier.end(), [e](WEdge *ec) {return e == ec;} ));
            }
        }
        //Caso na direção oposta
        else if (e->vend == start && e->vstart == end) {
            cout << "encontramos a aresta " << e->id << endl;
            e->cwloop = loop;
            loop->iedge = e;
            found = true;
            e->visits++;
            if (e->shouldRemove()) {
                frontier.erase(std::remove_if(frontier.begin(), frontier.end(), [e](WEdge *ec) {return e == ec;} ));
            }
        }

    }
    //Não encontramos, então vamos criar a aresta
    if (!found) {
        WEdge *newEdge = new WEdge(edgeCounter++);
        edges.push_back(newEdge);
        cout << "criamos a aresta " << newEdge->id << endl;

        newEdge->vstart = start;
        newEdge->vend = end;

        //TODO atualizar as arestas
//        newEdge->ccwsucc =
//        newEdge->ccwpred =
//        newEdge->cwsucc =
//        newEdge->cwpred =

        newEdge->ccwloop = loop;
        loop->iedge = newEdge;

        newEdge->type = WEdgeType::INTERNAL;
        newEdge->visits = 1;

        frontier.push_back(newEdge);
    }

    return frontier;
}

