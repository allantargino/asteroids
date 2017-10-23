#ifndef PHYSICS_H
#define PHYSICS_H

class Physics {
public:
    Physics();
    ~Physics();

    //Ship
    static constexpr float shipSize         = 0.10f;
    static constexpr float shipMoveAngle    = 10.00f; //degrees

    //Life
    static constexpr float shipLifeSize     = 0.05f;

    //Gunshot
    static constexpr float gunshotSize      = 0.02f;

    //Asteroid
    static constexpr float asteroidSSize    = 0.10f;
    static constexpr float asteroidMSize    = 0.15f;
    static constexpr float asteroidLSize    = 0.20f;

};
#endif // PHYSICS_H
