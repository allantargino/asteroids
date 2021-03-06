#ifndef ASTEROID_H
#define ASTEROID_H

#include "model.h"

class Asteroid : public Model
{
  public:
    Asteroid(QOpenGLWidget *_glWidget, std::shared_ptr<OffModel> _offModel, const GLuint &_shaderProgram, float _scale, const QVector3D &_initialPosition);
    ~Asteroid();

    float speed;
    bool isFragment;
};

#endif // ASTEROID_H
