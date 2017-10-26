#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include <QUuid>
#include <QQueue>
#include "ship.h"
#include "gunshot.h"
#include "asteroid.h"
#include "physics.h"

class ModelFactory : public QOpenGLExtraFunctions {
public:
    ModelFactory(QOpenGLWidget* _glWidget);
    ~ModelFactory();

    QOpenGLWidget* glWidget;

    //Shaders
    GLuint shaderProgramDefault = 0;
    GLuint shaderProgramEnergy = 0;
    GLuint createShaders(QString vertexShaderFile, QString fragmentShaderFile);
    void destroyShaders();

    std::shared_ptr<OffModel> shipOffModel = nullptr;
    std::shared_ptr<OffModel> gunshotOffModel = nullptr;
    std::shared_ptr<OffModel> asteroidOffModel = nullptr;

    std::shared_ptr<Ship> GetShipInstance();
    std::shared_ptr<Ship> GetScaledShipInstance(float size);
    std::shared_ptr<Gunshot> GetGunshotInstance(Ship* ship);
    std::shared_ptr<Asteroid> GetAsteroidInstance();
    std::vector<std::shared_ptr<Asteroid>> GetFragmentInstance(QVector3D initPosition, float fatherSize);

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
