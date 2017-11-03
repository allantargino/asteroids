#ifndef PHYSICS_H
#define PHYSICS_H

#include <QVector3D>
#include <QtMath>

class Physics {
public:
    Physics();
    ~Physics();

    //Ship
    static constexpr float shipSize             =    0.10f;
    static constexpr float shipMoveAngle        =   10.00f; //degrees
    static constexpr float shipAngleCorrection  =   90.00f; //degrees
    static constexpr float shipMovimentFactor   =    0.05f;

    //Life
    static constexpr float shipLifeSize         =   0.05f;

    //Gunshot
    static constexpr float gunshotSize              =    0.02f;
    static constexpr float gunshotAngleCorrection   =   90.00f; //degrees
    static constexpr float gunshotMovimentFactor    =    0.80f;

    //Asteroid
    static constexpr float asteroidSSize    = 0.10f;
    static constexpr float asteroidMSize    = 0.15f;
    static constexpr float asteroidLSize    = 0.20f;
    static constexpr float asteroidAngleCorrection  =    0.00f; //degrees
    static constexpr float asteroidMovimentFactor   =    0.50f;
    static constexpr float asteroidHighFactor       =    1.30f;
    static constexpr float asteroidLowFactor        =    0.70f;

    //Functions
    static QVector3D GetNextLinearMoviment(float currentX, float currentY, float currentAngle, float angleCorrection, float movimentFactor);
    static float GetRandomFactor(float high, float low);

};
#endif // PHYSICS_H
