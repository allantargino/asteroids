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

class Model : public QOpenGLExtraFunctions
{
  public:
    Model(QOpenGLWidget *_glWidget, std::shared_ptr<OffModel> _offModel, const GLuint &_shaderProgram, float _scale, const QVector3D &_initialPosition);
    ~Model();

    QOpenGLWidget *glWidget;

    std::shared_ptr<OffModel> offModel = nullptr;

    GLuint vao = 0;
    GLuint vboVertices = 0;
    GLuint vboIndices = 0;
    GLuint shaderProgram = 0;

    void createVBOs();
    void destroyVBOs();

    void drawModel();
    void drawModel(float arg);

    void Create();

    QMatrix4x4 modelMatrix;

    QVector3D initialPosition;
    QVector3D currentPosition;

    QString id;
    float scale;
    float hitBoxRadius;
    float angle;
    float color;

    bool CalculateColision(Model *other);
};

#endif // MODEL_H
