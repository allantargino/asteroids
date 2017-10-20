#include "modelfactory.h"

ModelFactory::ModelFactory(QOpenGLWidget* _glWidget)
{
    glWidget = _glWidget;

    QString shipFile = "C:\\Repos\\asteroids\\src\\models\\ship.off";
    shipOffModel = std::make_shared<OffModel>(shipFile);

    QString gunshotFile = "C:\\Repos\\asteroids\\src\\models\\sphere.off";
    gunshotOffModel = std::make_shared<OffModel>(gunshotFile);

    //QString shipFile = "C:\\Repos\\asteroids\\src\\models\\ship.off";
    //asteroidOffModel = std::make_shared<Asteroid>(shipFile);
}

ModelFactory::~ModelFactory(){}

std::shared_ptr<Ship> ModelFactory::GetShipInstance(){
    std::shared_ptr<Ship> ship = std::make_shared<Ship>(glWidget, shipOffModel);
    ship->Create();
    return ship;
}

std::shared_ptr<Gunshot> ModelFactory::GetGunshotInstance(){
    std::shared_ptr<Gunshot> gunshot = std::make_shared<Gunshot>(glWidget, gunshotOffModel);
    gunshot->Create();
    return gunshot;
}
