#ifndef PHYSICS_H
#define PHYSICS_H

#include "model.h"

class Physics {
public:
    Physics();
    ~Physics();

    bool CalculateColision(Model &A, Model &B);
};
#endif // PHYSICS_H
