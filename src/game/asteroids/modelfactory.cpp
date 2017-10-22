#include "modelfactory.h"

ModelFactory::ModelFactory(QOpenGLWidget* _glWidget)
{
    glWidget = _glWidget;

    QString shipFile = "C:\\Repos\\asteroids\\src\\models\\ship.off";
    shipOffModel = std::make_shared<OffModel>(shipFile);

    QString gunshotFile = "C:\\Repos\\asteroids\\src\\models\\sphere.off";
    gunshotOffModel = std::make_shared<OffModel>(gunshotFile);

    QString asteroidFile = "C:\\Repos\\asteroids\\src\\models\\sphere.off";
    asteroidOffModel = std::make_shared<OffModel>(asteroidFile);
}

ModelFactory::~ModelFactory(){}

std::shared_ptr<Ship> ModelFactory::GetShipInstance(){
    float size =  Physics::shipSize;
    std::shared_ptr<Ship> ship = std::make_shared<Ship>(glWidget, shipOffModel,size);
    ship->Create();

    ship->id = QUuid::createUuid().toString();

    return ship;
}

std::shared_ptr<Gunshot> ModelFactory::GetGunshotInstance(Ship* ship){
    float size =  Physics::gunshotSize;
    std::shared_ptr<Gunshot> gunshot = std::make_shared<Gunshot>(glWidget, gunshotOffModel, size);
    gunshot->Create();

    float x= ship->atualPoint.x() + 0.05*cos((ship->angle + 90)* (3.1416/180));
    float y= ship->atualPoint.y() + 0.05*sin((ship->angle + 90)* (3.1416/180));
    gunshot->atualPoint = QVector3D(x, y, 0);
    gunshot->angle = ship->angle;

    gunshot->id = QUuid::createUuid().toString();

    return gunshot;
}

std::shared_ptr<Asteroid> ModelFactory::GetAsteroidInstance(){
    float size = Physics::asteroidLSize;
    auto asteroid = std::make_shared<Asteroid>(glWidget, asteroidOffModel, size);
    asteroid->Create();

    int choice = qrand() % 2;
    int AbsSignalChoice = qPow(-1, (qrand() % 2));
    int HVChoise = qPow(-1, (qrand() % 2));
    int AngleSignalChoice = qPow(-1, (qrand() % 2));

    float Var = 1.2;
    float absChoice = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/Var));
    float Ang = 45.0;
    float angleChoice = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/Ang));

    QVector2D initPoint;
    if(choice == 0){ // y fixed
        initPoint = QVector2D(absChoice* AbsSignalChoice, Var*HVChoise);
    }else{ // x fixed
        initPoint = QVector2D(Var*HVChoise,absChoice * AbsSignalChoice);
    }

    QVector2D initVector = initPoint.normalized();
    float angle;
    //Angle definition pointing to origin
    if(initVector.y()>=0){
        QVector2D refVector(1,0);
        float dot = QVector2D::dotProduct(refVector,initVector);
        angle = qRadiansToDegrees(qAcos(dot)) + 180;
    }else{
        QVector2D refVector(-1,0);
        float dot = QVector2D::dotProduct(refVector,initVector);
        angle = qRadiansToDegrees(qAcos(dot));
    }
    //Random angle
    angle += angleChoice * AngleSignalChoice;

    asteroid->atualPoint = initPoint;
    asteroid->angle = angle;

    asteroid->id = QUuid::createUuid().toString();

    return asteroid;
}
