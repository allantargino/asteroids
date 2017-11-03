#include "lifemanager.h"

LifeManager::LifeManager(ModelFactory* _factory){
    factory = _factory;

    lastPosition = QVector2D(Physics::shipLifeUpperX, Physics::shipLifeUpperY);
}

LifeManager::~LifeManager(){}

void LifeManager::IncreaseLifeCount(){
    lastPosition =QVector2D(lastPosition.x()+ Physics::shipLifeUpperStep, lastPosition.y());

    float size = Physics::shipLifeSize;
    auto ship = factory->GetScaledShipInstance(size);
    ship->currentPosition = lastPosition;
    ships.push_back(ship);
}

void LifeManager::DecreaseLifeCount(){
    lastPosition =QVector2D(lastPosition.x()-Physics::shipLifeUpperStep, lastPosition.y());

    ships.pop_back();
}

void LifeManager::SetLifeCount(int newCount){
    ships.clear();
    lastPosition = QVector2D(Physics::shipLifeUpperX, Physics::shipLifeUpperY);

    for(int i =0; i<newCount; i++)
        IncreaseLifeCount();
}

int LifeManager::GetLifeCount(){
    return ships.size();
}

bool LifeManager::IsZero(){
    return ships.size() == 0;
}
