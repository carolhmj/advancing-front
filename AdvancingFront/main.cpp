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
    Vertex *v1 = new Vertex(1, glm::vec2(0,0));
    Vertex *v2 = new Vertex(2, glm::vec2(2,0));
    Vertex *v3 = new Vertex(3, glm::vec2(2,2));
    Vertex *v4 = new Vertex(4, glm::vec2(0,2));

    Edge *e1 = new Edge(v1, v2);
    Edge *e2 = new Edge(v2, v3);
    Edge *e3 = new Edge(v3, v4);
    Edge *e4 = new Edge(v4, v1);

    std::vector<Vertex*> vv = {v1,v2,v3,v4};
    std::vector<Edge*> ve = {e1,e2,e3,e4};

    Model *m = new Model(vv, ve);
    m->triangulate();
}

#endif
