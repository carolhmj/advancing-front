#include "model.h"
#include <algorithm>
#include <iostream>

Model::Model()
{

}

Model::Model(vector<Vertex> _vertices, vector<WEdge> _edges) : vertices(_vertices), edges(_edges) { }
//TODO colocar iedge nos pontos internos
void Model::triangulate()
{
    //Iniciamos com a fronteira recebendo a borda e as restrições
    vector<WEdge> frontier = edges;

    //Enquanto a fronteira não está vazia
    while (!frontier.empty()) {
        //Pega a próxima aresta a ser analisada
        WEdge *currEdge = &frontier.front();

        //Encontra o próximo ponto
        Vertex *point; // = find_point()

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
        Vertex *a = currEdge->vstart;
        Vertex *b = currEdge->vend;

        //Criar o novo triângulo
        loops.emplace_front();
        Loop *newLoop = &loops.front();

        currEdge->ccwloop = newLoop;

        findCreateEdge(b, point, newLoop, frontier);
        findCreateEdge(point, a, newLoop, frontier);
    }
}

vector<WEdge> Model::findCreateEdge(Vertex *start, Vertex *end, Loop *loop, vector<WEdge> frontier)
{
    vector<WEdge*> adj = start->adjedge();
    bool found = false;
    for (WEdge*& e : adj) {
        //Caso na direção que queremos
        if (e->vstart == start && e->vend == end) {
            e->ccwloop = newLoop;
            newLoop->iedge = e;
            found = true;
            e->visits++;
            if (e->shouldRemove()) {
                frontier.erase(std::remove_if(frontier.begin(), frontier.end(), [](WEdge ec) {return e == &ec;} ));
            }
        }
        //Caso na direção oposta
        else if (e->vend == start && e->vstart == end) {
            e->cwloop = newLoop;
            newLoop->iedge = e;
            found = true;
            e->visits++;
            if (e->shouldRemove()) {
                frontier.erase(std::remove_if(frontier.begin(), frontier.end(), [](WEdge ec) {return e == &ec;} ));
            }
        }

    }
    //Não encontramos, então vamos criar a aresta
    if (!found) {
        edges.emplace_front();
        WEdge *newEdge = &edges.front();

        newEdge->vstart = start;
        newEdge->vend = end;

        newEdge->ccwloop = newLoop;
        newLoop->iedge = newEdge;

        newEdge->type = WEdgeType::INTERNAL;
        newEdge->visits = 1;

        frontier.push_back(*newEdge);
    }

    return frontier;
}

