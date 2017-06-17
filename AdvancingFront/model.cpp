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
        //TODO só checar os pontos à direita da aresta
        for (Vertex*& v : vertices) {
            if (v != a && v != b && Primitives::isRightTo(currEdge->value(), v->pos - a->pos)) {
                cout << v->id << " right to " << Primitives::isRightTo(currEdge->value(), v->pos - a->pos) << endl;
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
        Face *newFace = new Face(currEdge->a, currEdge->b, point);
        faces.push_back(newFace);

        currEdge->visits++;

        if (currEdge->shouldRemove()) {
            cout << "remove a aresta " << currEdge->print() << "da fronteira\n";
            frontier.erase(std::remove_if(frontier.begin(), frontier.end(), [currEdge](Edge *ec) {return currEdge == ec;} ));
        }

        //Referências às arestas que serão encontradas/criadas
        Edge *bpEdge;
        Edge *paEdge;

        frontier = findCreateEdge(a, point, frontier, bpEdge);
        frontier = findCreateEdge(point, b, frontier, paEdge);

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
        //Caso na direção que queremos, não criamos nada!
        if (e->a == start && e->b == end) {
            cout << "aresta na msm direção " << e->print()<< endl;
            found = true;
        }
        //Caso na direção oposta
        else if (e->b == start && e->a == end) {
            cout << "aresta na direção oposta " << e->print() << endl;
            //Como encontramos a aresta na direção oposta, visitamos a aresta 2 vezes, portanto, vamos apagá-la
            frontier.erase(std::remove_if(frontier.begin(), frontier.end(), [e](Edge *ec) {return e == ec;} ));
            found = true;
        }

    }
    //Não encontramos, então vamos criar a aresta
    if (!found) {
        newEdge = new Edge(start, end);
        edges.push_back(newEdge);
        frontier.push_back(newEdge);
        cout << "criamos a aresta " << newEdge->print() << endl;

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

void Model::draw()
{
    //Desenha os pontos
    glColor3f(1,0,0);
    glPointSize(5);
    glBegin(GL_POINTS);
        for (Vertex*& v : vertices) {
            glVertex2f(v->pos.x, v->pos.y);
        }
    glEnd();
    glPointSize(1);
    //Desenha as arestas
    glColor3f(0,1,0);
    glBegin(GL_LINES);
        for (Edge*& e : edges) {
            glVertex2f(e->a->pos.x, e->a->pos.y);
            glVertex2f(e->b->pos.y, e->b->pos.y);
        }
    glEnd();
    //Desenha os triângulos
}

