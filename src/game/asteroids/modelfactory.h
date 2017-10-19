#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include "ship.h"
#include "gunshot.h"

class ModelFactory {
public:
    ModelFactory();
    ~ModelFactory();

    std::shared_ptr<Ship> GetShipInstance();
    std::shared_ptr<Gunshot> GetGunshotInstance();
    //std::shared_ptr<Asteroid> GetAsteroidInstance();
};


#endif // MODELFACTORY_H
