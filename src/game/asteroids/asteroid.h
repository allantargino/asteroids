#ifndef ASTEROID_H
#define ASTEROID_H

#include "model.h"

class Asteroid : public Model {
public:
    Asteroid(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, GLuint _shaderProgram, float _scale, QVector3D _initialPosition);
    ~Asteroid();

    float speed;
};


#endif // ASTEROID_H
