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

    WEdge *e1 = new WEdge(1);
    WEdge *e2 = new WEdge(2);
    WEdge *e3 = new WEdge(3);
    WEdge *e4 = new WEdge(4);

    e1->vstart = v1;
    e1->vend = v2;
    e2->vstart = v2;
    e2->vend = v3;
    e3->vstart = v3;
    e3->vend = v4;
    e4->vstart = v4;
    e4->vend = v1;

    v1->iedge = e1;
    v2->iedge = e2;
    v3->iedge = e3;
    v4->iedge = e4;

    e1->type = WEdgeType::BORDER;
    e2->type = WEdgeType::BORDER;
    e3->type = WEdgeType::BORDER;
    e4->type = WEdgeType::BORDER;

    e1->ccwsucc = e2;
    e1->ccwpred = e4;
    e1->cwsucc = e4;
    e1->cwpred = e2;

    e2->ccwsucc = e3;
    e2->ccwpred = e1;
    e2->cwsucc = e1;
    e2->cwpred = e3;

    e3->ccwsucc = e4;
    e3->ccwpred = e3;
    e3->cwsucc = e3;
    e3->cwpred = e4;

    e4->ccwsucc = e1;
    e4->ccwpred = e3;
    e4->cwsucc = e3;
    e4->cwpred = e1;

    e1->ccwloop = nullptr;
    e1->cwloop = nullptr;

    e2->ccwloop = nullptr;
    e2->cwloop = nullptr;

    e3->ccwloop = nullptr;
    e3->cwloop = nullptr;

    e4->ccwloop = nullptr;
    e4->cwloop = nullptr;

    std::vector<Vertex*> vv = {v1,v2,v3,v4};
    std::vector<WEdge*> ve = {e1,e2,e3,e4};

    Model *m = new Model(vv, ve);
    m->triangulate();
}

#endif
