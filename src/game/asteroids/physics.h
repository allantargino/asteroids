#ifndef PHYSICS_H
#define PHYSICS_H

class Physics {
public:
    Physics();
    ~Physics();

    static constexpr float shipSize       = 0.10f;
    static constexpr float shipMoveAngle  = 5.00f;

    static constexpr float gunshotSize    = 0.02f;


    static constexpr float asteroidSSize    = 0.2f;
    static constexpr float asteroidMSize    = 0.2f;
    static constexpr float asteroidLSize    = 0.2f;

};
#endif // PHYSICS_H
