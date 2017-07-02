#include "glwidget.h"
#include <iostream>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <GL/glu.h>

using std::cout;
using std::endl;

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {
}

void GLWidget::triangulateModel()
{
    for (Model*& model : models) {
        std::cout << "--------------------\n";
        model->triangulate();
    }
    update();
}

void GLWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

    for (Model*& model : models) {
        model->draw();
    }
}

void GLWidget::adjustOrtho()
{
    if (models.empty()) {
        projection = glm::ortho(-1,1,-1,1);
    } else {
        float maxCoordX = std::numeric_limits<float>::lowest();
        float minCoordX = std::numeric_limits<float>::max();
        float maxCoordY = std::numeric_limits<float>::lowest();
        float minCoordY = std::numeric_limits<float>::max();
        for (Model*& model : models) {
            float modelMaxX = model->getMaxXCoordValue();
            float modelMinX = model->getMinXCoordValue();
            maxCoordX = (modelMaxX >= maxCoordX) ? modelMaxX : maxCoordX;
            minCoordX = (modelMinX <= minCoordX) ? modelMinX : minCoordX;
            float modelMaxY = model->getMaxYCoordValue();
            float modelMinY = model->getMinYCoordValue();
            maxCoordY = (modelMaxY >= maxCoordY) ? modelMaxY : maxCoordY;
            minCoordY = (modelMinY <= minCoordY) ? modelMinY : minCoordY;
        }

        float xDiff = maxCoordX - minCoordX;
        float yDiff = maxCoordY - minCoordY;

        projection = glm::ortho(minCoordX - 0.1*xDiff, maxCoordX + 0.1*xDiff, minCoordY - 0.1*yDiff, maxCoordY + 0.1*yDiff);
    }
}

void GLWidget::openArchive() {
    models.clear();
    std::vector<std::vector<Vertex*>> verticesVector;
    std::vector<std::vector<Edge*>> edgesVector;

    //First, we'll open the vertex file
    QString fileNameV = QFileDialog::getOpenFileName(this,
        tr("Open Vertex File"), "/home", tr(""));

    std::cout << "opening vertex file: " << fileNameV.toStdString() << "\n";

    QFile fileV(fileNameV);
    if (!fileV.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream inV(&fileV);

    int numGroupsV = inV.readLine().toInt();

    try {

        //Lê cada grupo
        for (int i = 0; i < numGroupsV; i++) {
            std::vector<Vertex*> vertices;
            int numPoints = inV.readLine().toInt();
            for (int j = 0; j < numPoints; j++) {
                float id, x, y, z;
                QString point = inV.readLine();
                QStringList info = point.split(" ");
                id = info[0].toInt();
                x = info[1].toFloat();
                y = info[2].toFloat();
                z = info[3].toFloat();
//                std::cout << "Read vertex " << id << ": " << x << " " << y << " " << z << "\n";
                std::flush(std::cout);
                vertices.push_back(new Vertex(id, {x,y}));
            }
            verticesVector.push_back(vertices);
        }

    } catch (std::exception& e) {
        std::cout << "Erro na leitura de arquivo!\n";
    }

    fileV.close();

    //Then, we'll read the edges
    QString fileNameE = QFileDialog::getOpenFileName(this,
        tr("Open Edge File"), "/home", tr(""));

    std::cout << "opening edge file: " << fileNameE.toStdString() << "\n";

    QFile fileE(fileNameE);
    if (!fileE.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream inE(&fileE);

    int numGroupsE = inE.readLine().toInt();

    try {
        for (int i = 0; i < numGroupsE; i++) {
            //if i = ... (controle de grupos)
            std::vector<Edge*> edges;
            int numEdges = inE.readLine().toInt();
            for (int j = 0; j < numEdges; j++) {
                float idA, idB;
                QString edge = inE.readLine();
                QStringList info = edge.split(" ");
                idA = info[0].toInt();
                idB = info[1].toInt();
                Vertex *vA, *vB;
                vA = findVertexIdInVector(idA, verticesVector[i]);
                vB = findVertexIdInVector(idB, verticesVector[i]);
                edges.push_back(new Edge(vA, vB));
            }
            edgesVector.push_back(edges);
        }
    } catch (std::exception& e) {
        std::cout << "Erro na leitura de arquivo!\n";
    }

    for (int i = 0; i < verticesVector.size(); i++) {
        Model *newModel = new Model(verticesVector[i], edgesVector[i]);
//        std::cout << newModel->print();
        models.push_back(newModel);
    }

    adjustOrtho();
    update();
}

Vertex *GLWidget::findVertexIdInVector(unsigned int id, std::vector<Vertex *> vector)
{
    for (Vertex*& v : vector) {
        if (v->id == id) {
            return v;
        }
    }
    return nullptr;
}
