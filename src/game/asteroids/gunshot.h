#ifndef GUNSHOT_H
#define GUNSHOT_H

#include "model.h"

class Gunshot : public Model
{
  public:
    Gunshot(QOpenGLWidget *_glWidget, std::shared_ptr<OffModel> _offModel, const GLuint &_shaderProgram, float _scale, const QVector3D &_initialPosition);
    ~Gunshot();
};

#endif // GUNSHOT_H
