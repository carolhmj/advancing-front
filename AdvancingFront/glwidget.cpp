#include "glwidget.h"
#include <iostream>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>

using std::cout;
using std::endl;

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {
    vector<Vertex*> vv;
    vv.push_back(new Vertex(1, {0,8}));
    vv.push_back(new Vertex(2, {-4,6}));
    vv.push_back(new Vertex(3, {-5,0}));
    vv.push_back(new Vertex(4, {-1,1}));
    vv.push_back(new Vertex(5, {-3,-3}));
    vv.push_back(new Vertex(6, {0,-4}));
    vv.push_back(new Vertex(7, {5,-2}));
    vv.push_back(new Vertex(8, {9,0}));
    vv.push_back(new Vertex(9, {4,5}));
    vv.push_back(new Vertex(10, {8,3}));

//    vv.push_back(new Vertex(1, {0,0}));
//    vv.push_back(new Vertex(2, {0,1}));
//    vv.push_back(new Vertex(3, {1,1}));
//    vv.push_back(new Vertex(4, {1,0}));

    vector<Edge*> ve;
    ve.push_back(new Edge(vv[0], vv[9]));
    ve.push_back(new Edge(vv[9], vv[7]));
    ve.push_back(new Edge(vv[7], vv[6]));
    ve.push_back(new Edge(vv[6], vv[5]));
    ve.push_back(new Edge(vv[5], vv[4]));
    ve.push_back(new Edge(vv[4], vv[2]));
    ve.push_back(new Edge(vv[2], vv[1]));
    ve.push_back(new Edge(vv[1], vv[0]));
    //Restrição
    ve.push_back(new Edge(vv[8], vv[5]));
    ve.push_back(new Edge(vv[5], vv[8]));
    ve.push_back(new Edge(vv[1], vv[4]));
    ve.push_back(new Edge(vv[4], vv[1]));
//    ve.push_back(new Edge(vv[0],vv[1]));
//    ve.push_back(new Edge(vv[1],vv[2]));
//    ve.push_back(new Edge(vv[2],vv[3]));
//    ve.push_back(new Edge(vv[3],vv[0]));

    Model *model = new Model(vv, ve);
    cout << model->print() << endl;
    models.push_back(model);
    adjustOrtho();
}

void GLWidget::triangulateModel()
{
    for (Model*& model : models) {
        model->triangulate();
    }
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

    for (Model*& model : models) {
        model->draw();
    }
}

void GLWidget::adjustOrtho()
{
    if (models.empty()) {
        projection = glm::ortho(-1,1,-1,1);
    } else {
        float maxCoord = std::numeric_limits<float>::lowest();
        for (Model*& model : models) {
            float modelMax = model->getMaxCoordValue();
            maxCoord = (modelMax >= maxCoord) ? modelMax : maxCoord;
        }
        float border = 0.1*maxCoord;
        float maxTotal = maxCoord + border;
        projection = glm::ortho(-maxTotal, maxTotal, -maxTotal, maxTotal);
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
                std::cout << "Read vertex " << id << ": " << x << " " << y << " " << z << "\n";
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
            EType edgesType = EType::BORDER;
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
                edges.push_back(new Edge(vA, vB, edgesType));
            }
            edgesVector.push_back(edges);
        }
    } catch (std::exception& e) {
        std::cout << "Erro na leitura de arquivo!\n";
    }

    for (int i = 0; i < verticesVector.size(); i++) {
        Model *newModel = new Model(verticesVector[i], edgesVector[i]);
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
