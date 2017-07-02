#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QKeyEvent>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model.h"

struct Camera {
    glm::vec3 eye = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};

class GLWidget : public QOpenGLWidget
{
public:
    explicit GLWidget(QWidget *parent = 0);
    void triangulateModel();
    void openArchive();

private:
    std::vector<Model*> models;
    Camera camera;
    glm::mat4 view;
    glm::mat4 projection;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void adjustOrtho();
    Vertex* findVertexIdInVector(unsigned int id, std::vector<Vertex*> vector);

    void keyPressEvent(QKeyEvent *event);

};

#endif // GLWIDGET_H
