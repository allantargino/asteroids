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

    ship->id = QUuid::createUuid().toString();

    return ship;
}

std::shared_ptr<Gunshot> ModelFactory::GetGunshotInstance(Ship* ship){
    std::shared_ptr<Gunshot> gunshot = std::make_shared<Gunshot>(glWidget, gunshotOffModel);
    gunshot->Create();

    float x= ship->atualPoint.x() + 0.05*cos((ship->angle + 90)* (3.1416/180));
    float y= ship->atualPoint.y() + 0.05*sin((ship->angle + 90)* (3.1416/180));
    gunshot->atualPoint = QVector3D(x, y, 0);
    gunshot->angle = ship->angle;

    gunshot->id = QUuid::createUuid().toString();

    return gunshot;
}
