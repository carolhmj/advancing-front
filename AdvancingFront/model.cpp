#include "model.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include "glm/gtx/string_cast.hpp"

using std::cout;
using std::endl;

Model::Model()
{

}

Model::Model(vector<Vertex *> _vertices, vector<Edge *> _edges) : vertices(_vertices), edges(_edges) {
    edgeCounter = _edges.size() + 1;
    faceCounter = 1;
}

void Model::triangulate()
{
    //Iniciamos com a fronteira recebendo a borda e as restrições
    vector<Edge*> frontier = edges;

    //Enquanto a fronteira não está vazia
    while (!frontier.empty()) {
//        std::cout << "Examining frontier:" << endl;
//        for (Edge*& e : frontier) {
//            cout << e->print() << endl;
//        }
        //Pega a próxima aresta a ser analisada
        Edge *currEdge = frontier.front();

        cout << "curr edge: " << currEdge->print() << endl;

        Vertex *a = currEdge->a;
        Vertex *b = currEdge->b;

        //Encontra o próximo ponto
        Vertex *point = nullptr;

        float maxAngle = std::numeric_limits<float>::lowest();

        //Vamos utilizar o critério de delaunay (maior ângulo) com todos os pontos
        bool vertexInRight = false;
        bool noIntersection = false;
        for (Vertex*& v : vertices) {
            if (v != a && v != b) {
                if (Primitives::isRightTo(currEdge->value(), v->pos - a->pos)) {
                    //Algum vértice está a direita da aresta
                    vertexInRight = true;
                    if (!intersectsFrontier(v->pos, a->pos, frontier) && !intersectsFrontier(b->pos, v->pos, frontier)) {
                        //Algum vértice permite criar o triângulo sem interseção na fronteira
                        noIntersection = true;
                        float angle = Primitives::angle(a->pos - v->pos, b->pos - v->pos);
                        if (angle >= maxAngle) {
                            point = v;
                            maxAngle = angle;
                        }
                    }
                }
            }
        }

        //Caso todos os pontos tenham interseção então não é triangularizável
        if (vertexInRight && !noIntersection) {
            std::cout << "Modelo não pode ser triangularizado!\n";
            std::flush(std::cout);
            return;
        }
        //Tira a aresta da fronteira
        frontier.erase(std::remove_if(frontier.begin(), frontier.end(), [currEdge](Edge *ec) {return currEdge == ec;} ));
        if (noIntersection) {

            //Criar o novo triângulo
            Face *newFace = new Face(currEdge->a, currEdge->b, point);
            faces.push_back(newFace);


            //Referências às arestas que serão encontradas/criadas
            Edge *bpEdge;
            Edge *paEdge;

            frontier = findCreateEdge(a, point, frontier, bpEdge);
            frontier = findCreateEdge(point, b, frontier, paEdge);
        }

    }

//    cout << "triangulate ended with faces " << endl;
//    for (Face*& f : faces) {
//        cout << f->print() << endl;
//    }
}

vector<Edge*> Model::findCreateEdge(Vertex *start, Vertex *end, vector<Edge*> frontier, Edge *newEdge)
{
    vector<Edge*> adj = findAdjEdges(start, frontier);
    bool found = false;

    for (Edge*& e : adj) {
        //Caso na direção que queremos, não criamos nada!
        if (e->a == start && e->b == end) {
//            cout << "aresta na msm direção " << e->print()<< endl;
            found = true;
        }
        //Caso na direção oposta
        else if (e->b == start && e->a == end) {
//            cout << "aresta na direção oposta " << e->print() << endl;
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
//        cout << "criamos a aresta " << newEdge->print() << endl;

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

bool Model::intersectsFrontier(glm::vec2 edgeA, glm::vec2 edgeB, vector<Edge *> frontier)
{
    for (Edge*& e : frontier) {
        if (Primitives::intersects(e->a->pos, e->b->pos, edgeA, edgeB)) {
//            cout << "edge " << e->a->id << " " << e->b->id << " intersects" << endl;
            return true;
        }
    }

    return false;
}

void Model::draw()
{
    //Desenha os pontos
    glColor3f(1,0,0);
    glPointSize(3);
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
            glVertex2f(e->b->pos.x, e->b->pos.y);
        }
    glEnd();
    //Desenha os triângulos
}

float Model::getMaxCoordValue()
{
    float max = std::numeric_limits<float>::lowest();
    for (Vertex*& v : vertices) {
        max = (std::abs(v->pos.x) >= max)? std::abs(v->pos.x) : max;
        max = (std::abs(v->pos.y) >= max)? std::abs(v->pos.y) : max;
    }

    cout << "max is " << max << endl;
    return max;
}

std::string Model::print()
{
    std::stringstream ss;
    ss << "VERTICES =========" << endl;
    for (Vertex*& v : vertices) {
        ss << v->print() << endl;
    }
    ss << "EDGES ==========" << endl;
    for (Edge*& e : edges) {
        ss << e->print() << endl;
    }
    ss << "FACES ===========" << endl;
    for (Face*& f : faces) {
        ss << f->print() << endl;
    }
    return ss.str();
}

