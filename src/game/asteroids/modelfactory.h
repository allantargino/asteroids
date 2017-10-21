#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include "ship.h"
#include "gunshot.h"

class ModelFactory {
public:
    ModelFactory(QOpenGLWidget* _glWidget);
    ~ModelFactory();

    QOpenGLWidget* glWidget;

    std::shared_ptr<OffModel> shipOffModel = nullptr;
    std::shared_ptr<OffModel> gunshotOffModel = nullptr;
    //std::shared_ptr<OffModel> asteroidOffModel = nullptr;

    std::shared_ptr<Ship> GetShipInstance();
    std::shared_ptr<Gunshot> GetGunshotInstance(Ship* ship);
    //std::shared_ptr<Asteroid> GetAsteroidInstance();
};


#endif // MODELFACTORY_H
