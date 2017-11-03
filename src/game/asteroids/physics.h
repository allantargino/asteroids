#ifndef PHYSICS_H
#define PHYSICS_H

#include <QVector3D>
#include <QtMath>

class Physics {
public:
    Physics();
    ~Physics();

    //Model
static constexpr float modelInitialColor             =    1.00f;
static constexpr float modelInitialAngle             =    0.00f;

    //Ship
    static constexpr float shipSize             =    0.10f;
    static constexpr float shipMoveAngle        =   10.00f; //degrees
    static constexpr float shipAngleCorrection  =   90.00f; //degrees
    static constexpr float shipMovimentFactor   =    0.05f;

    //Life
    static constexpr float shipLifeSize         =   0.05f;
static constexpr float shipLifeUpperX        =   -1.00f;
static constexpr float shipLifeUpperY        =   0.93f;
static constexpr float shipLifeUpperStep        =   0.05f;

    //Gunshot
    static constexpr float gunshotSize              =    0.02f;
    static constexpr float gunshotAngleCorrection   =   90.00f; //degrees
    static constexpr float gunshotMovimentFactor    =    0.80f;

    //Asteroid
    static constexpr float asteroidSize                     =    0.15f;
    static constexpr float asteroidSizeHighFactor           =    2.00f;
    static constexpr float asteroidSizeLowFactor            =    0.50f;
    static constexpr float asteroidAngleCorrection          =    0.00f; //degrees
    static constexpr float asteroidMovimentFactor           =    0.50f;
    static constexpr float asteroidMovimentHighFactor       =    1.30f;
    static constexpr float asteroidMovimentLowFactor        =    0.70f;

    //Fragment
    static constexpr float  fragmentBooster                 =    0.50f;
    static constexpr float  fragmentAngleHighFactor         =   90.00f; //degrees
    static constexpr float  fragmentAngleLowFactor          =    0.00f; //degrees
    static constexpr int    fragmentQuantityInitial         =    3;
    static constexpr int    fragmentQuantityAdditionalMax   =    4;

    //Factory
    static constexpr float  factoryAsteroidInstancePosition =    1.20f;
    static constexpr float  factoryAsteroidInstanceAngle    =   45.00f; //degrees
static constexpr int  factoryAsteroidQuantity    =  250 ;
static constexpr int  factoryGunshotQuantity    =  100 ;
static constexpr float fragmentColorFactor = 6.00f;


    //Game
    static constexpr int gameInitialLifes = 5;
    static constexpr float  gameInitialAsteroidTimeRelease =    2.00f; //seconds
    static constexpr int  gameNextLevelFactor    =  10 ;
    static constexpr float gameOutboundAsteroidPosition = 1.40f;
static constexpr float gameOutboundGunshotPosition = 1.20f;
    static constexpr int gameVolumeShipPlayer = 100;
    static constexpr int gameVolumeShotPlayer = 100;
        static constexpr int gameVolumeAsteroidPlayer = 100;

    //Functions
    static QVector3D GetNextLinearMoviment(float currentX, float currentY, float currentAngle, float angleCorrection, float movimentFactor);
    static float GetRandomFactor(float high, float low);

};
#endif // PHYSICS_H
