#include "model.h"

Model::Model()
{

}

Model::Model(vector<Vertex> _vertices, vector<WEdge> _edges) : vertices(_vertices), edges(_edges) { }

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

        vertex<WEdge*> adj = point->adjedge();
        bool found = false;
        for (WEdge*& e : adj) {
            //Caso na direção que queremos
            if (e->vstart == a && e->vend == b) {

                found = true;
                e->visits++;
                if (e->shouldRemove()) {

                }
            }
            //Caso na direção oposta
            else if (e->vend == a && e->vstart == b) {
                found = true;
            }

        }
        //Não encontramos, então vamos criar a aresta
        if (!found) {
            edges.emplace_front();
            WEdge *newEdge = &edges.front();

            newEdge->vstart = b;
            newEdge->vend = point;

            //TODO ver se é ccw ou cw
            newEdge->ccwloop = newLoop;

            newEdge->type = WEdgeType::INTERNAL;
            newEdge->visits = 1;

            frontier.push_back(*newEdge);
        }

    }
}

