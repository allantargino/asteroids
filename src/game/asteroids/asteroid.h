#ifndef ASTEROID_H
#define ASTEROID_H

#include "model.h"

class Asteroid : public Model {
public:
    Asteroid(QOpenGLWidget* _glWidget, std::shared_ptr<OffModel> _offModel, float _scale, QString _vertexShaderFile, QString _fragmentShaderFile, QVector3D _initialPosition);
    ~Asteroid();
};


#endif // ASTEROID_H
