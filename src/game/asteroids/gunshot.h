#ifndef GUNSHOT_H
#define GUNSHOT_H

#include "model.h"

class Gunshot : public Model {
public:
    Gunshot(QOpenGLWidget* _glWidget);
    ~Gunshot();
};

#endif // GUNSHOT_H
