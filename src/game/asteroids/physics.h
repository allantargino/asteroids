#ifndef PHYSICS_H
#define PHYSICS_H

#include "model.h"

class Physics {
public:
    Physics();
    ~Physics();

    static constexpr float shipSize       = 0.10f;
    static constexpr float gunshotSize    = 0.02f;

    bool CalculateColision(Model &A, Model &B);
};
#endif // PHYSICS_H
