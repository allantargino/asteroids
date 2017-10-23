#ifndef ASTEROID_H
#define ASTEROID_H

#include "model.h"

class Asteroid : public Model {
public:
    Asteroid(QOpenGLWidget* _glWidget, std::shared_ptr<OffModel> _offModel, float _scale);
    ~Asteroid();
};


#endif // ASTEROID_H
