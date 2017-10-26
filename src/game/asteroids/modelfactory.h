#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include <QUuid>
#include <QQueue>
#include "ship.h"
#include "gunshot.h"
#include "asteroid.h"
#include "physics.h"

class ModelFactory {
public:
    ModelFactory(QOpenGLWidget* _glWidget);
    ~ModelFactory();

    QOpenGLWidget* glWidget;

    std::shared_ptr<OffModel> shipOffModel = nullptr;
    std::shared_ptr<OffModel> gunshotOffModel = nullptr;
    std::shared_ptr<OffModel> asteroidOffModel = nullptr;

    std::shared_ptr<Ship> GetShipInstance();
    std::shared_ptr<Ship> GetScaledShipInstance(float size);
    std::shared_ptr<Gunshot> GetGunshotInstance(Ship* ship);
    std::shared_ptr<Asteroid> GetAsteroidInstance();

    void RemoveAsteroidInstance(std::shared_ptr<Asteroid>);
    void RemoveGunshotInstance(std::shared_ptr<Gunshot>);

    void LoadInstances();

private:
    bool isInitialized = false;

    QQueue<std::shared_ptr<Asteroid>> AsteroidQueue;
    QQueue<std::shared_ptr<Gunshot>> GunshotQueue;

    std::shared_ptr<Asteroid> CreateAsteroidInstance();
    std::shared_ptr<Gunshot> CreateGunshotInstance();

    void LoadAsteroidInstances();
    void LoadGunshotInstances();
};


#endif // MODELFACTORY_H
