#include "glwidget.h"
#include <iostream>

using std::cout;
using std::endl;

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {
    vector<Vertex*> vv;
//    vv.push_back(new Vertex(1, {0,2}));
//    vv.push_back(new Vertex(2, {-4,6}));
//    vv.push_back(new Vertex(3, {-5,0}));
//    vv.push_back(new Vertex(4, {-1,1}));
//    vv.push_back(new Vertex(5, {-3,-3}));
//    vv.push_back(new Vertex(6, {0,-4}));
//    vv.push_back(new Vertex(7, {5,-2}));
//    vv.push_back(new Vertex(8, {9,0}));
//    vv.push_back(new Vertex(9, {4,5}));
//    vv.push_back(new Vertex(10, {8,3}));

    vv.push_back(new Vertex(1, {0,0}));
    vv.push_back(new Vertex(2, {0,1}));
    vv.push_back(new Vertex(3, {1,1}));
    vv.push_back(new Vertex(4, {1,0}));

    vector<Edge*> ve;
//    ve.push_back(new Edge(vv[0], vv[9]));
//    ve.push_back(new Edge(vv[9], vv[7]));
//    ve.push_back(new Edge(vv[7], vv[6]));
//    ve.push_back(new Edge(vv[6], vv[5]));
//    ve.push_back(new Edge(vv[5], vv[4]));
//    ve.push_back(new Edge(vv[4], vv[2]));
//    ve.push_back(new Edge(vv[2], vv[1]));
//    ve.push_back(new Edge(vv[1], vv[0]));
    ve.push_back(new Edge(vv[0],vv[1]));
    ve.push_back(new Edge(vv[1],vv[2]));
    ve.push_back(new Edge(vv[2],vv[3]));
    ve.push_back(new Edge(vv[3],vv[0]));

    model = new Model(vv, ve);
    cout << model->print() << endl;
    adjustOrtho();
}

void GLWidget::triangulateModel()
{
    model->triangulate();
    update();
}

void GLWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    view = glm::lookAt(camera.eye, camera.at, camera.up);
}

void GLWidget::resizeGL(int w, int h) {
    if(h<w)
        glViewport((w-h)/2,0,h,h);
    else
        glViewport(0,(h-w)/2,w,w);
}

void GLWidget::paintGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));

    if (model) model->draw();

    glBegin(GL_LINES);
    glVertex2f(0,0);
    glVertex2f(0,1);
    glEnd();
}

void GLWidget::adjustOrtho()
{
    if (model) {
        float max = model->getMaxCoordValue();
        float border = 0.1*max;
        float maxTotal = max + border;
        projection = glm::ortho(-maxTotal, maxTotal, -maxTotal, maxTotal);
    } else {
        projection = glm::ortho(-1,1,-1,1);
    }
}
