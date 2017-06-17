#if INTERFACE

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

#endif

#if !INTERFACE

#include "model.h"

int main(int argc, char *argv[])
{
    Vertex *v1 = new Vertex(1, glm::vec2(0,8));
    Vertex *v2 = new Vertex(2, glm::vec2(-4,6));
    Vertex *v3 = new Vertex(3, glm::vec2(-5,0));
    Vertex *v4 = new Vertex(4, glm::vec2(-1,1));
    Vertex *v5 = new Vertex(5, glm::vec2(-3,-3));
    Vertex *v6 = new Vertex(6, glm::vec2(0,-4));
    Vertex *v7 = new Vertex(7, glm::vec2(5,-2));
    Vertex *v8 = new Vertex(8, glm::vec2(9,0));
    Vertex *v9 = new Vertex(9, glm::vec2(4,5));
    Vertex *v10 = new Vertex(10, glm::vec2(8,3));

    Edge *e1 = new Edge(v7, v6);
    Edge *e2 = new Edge(v8, v7);
    Edge *e3 = new Edge(v10, v8);
    Edge *e4 = new Edge(v1, v10);
    Edge *e5 = new Edge(v2, v1);
    Edge *e6 = new Edge(v3, v2);
    Edge *e7 = new Edge(v5, v3);
    Edge *e8 = new Edge(v6, v5);

    std::vector<Vertex*> vv = {v1,v2,v3,v4,v5,v6,v7,v8,v9,v10};
    std::vector<Edge*> ve = {e1,e2,e3,e4,e5,e6,e7,e8};

//    Vertex *v2 = new Vertex(2, glm::vec2(-4,6));
//    Vertex *v3 = new Vertex(3, glm::vec2(-5,0));
//    Vertex *v4 = new Vertex(4, glm::vec2(-1,1));
//    Vertex *v5 = new Vertex(5, glm::vec2(-3,-3));

//    Edge *e1 = new Edge(v3,v2);
//    Edge *e2 = new Edge(v2,v4);
//    Edge *e3 = new Edge(v4,v5);
//    Edge *e4 = new Edge(v5,v3);

//    std::vector<Vertex*> vv = {v2, v3,v4,v5};
//    std::vector<Edge*> ve = {e1,e2,e3,e4};

    Model *m = new Model(vv, ve);
    m->triangulate();
}

#endif
