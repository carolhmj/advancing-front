#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QKeyEvent>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model.h"

class GLWidget : public QOpenGLWidget
{
public:
    explicit GLWidget(QWidget *parent = 0);
    void triangulateModel();
    void openArchive();

private:
    std::vector<Model*> models;
    glm::mat4 view;
    glm::mat4 projection;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void adjustOrtho();
    Vertex* findVertexIdInVector(unsigned int id, std::vector<Vertex*> vector);
};

#endif // GLWIDGET_H
