#ifndef MODEL_H
#define MODEL_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTextStream>
#include <QFile>
#include <fstream>
#include <limits>
#include <iostream>
#include <memory>
#include "offmodel.h"
#include "physics.h"

class Model : public QOpenGLExtraFunctions {
public:
    Model(QOpenGLWidget* _glWidget, std::shared_ptr<OffModel> _offModel, float _scale);
    ~Model();

    QOpenGLWidget* glWidget;

    std::shared_ptr<OffModel> offModel = nullptr;

    GLuint vao = 0;
    GLuint vboVertices = 0;
    GLuint vboIndices = 0;
    GLuint shaderProgram = 0;

    void createVBOs();
    void createShaders();
    void destroyVBOs();
    void destroyShaders();
    void drawModel();
    void Create();

    QMatrix4x4 modelMatrix;
    QVector3D atualPoint;

    QString id;
    float scale;
    float hitBoxRadius;
    float angle;

    bool CalculateColision(Model* other);
};

#endif // MODEL_H
