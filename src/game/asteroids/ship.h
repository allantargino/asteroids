#ifndef SHIP_H
#define SHIP_H

#include "model.h"

class Ship : public Model {
public:
    Ship(QOpenGLWidget* _glWidget, std::shared_ptr<OffModel> _offModel);
    ~Ship();
};

#endif // SHIP_H
