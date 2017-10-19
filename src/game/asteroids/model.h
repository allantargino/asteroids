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

class Model : public QOpenGLExtraFunctions {
public:
    Model(QOpenGLWidget* _glWidget, std::shared_ptr<OffModel> _offModel);
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
    void drawModel(float angle, float scale);
    void Create();

    QMatrix4x4 modelMatrix;
    QVector3D atualPoint;
};

#endif // MODEL_H
