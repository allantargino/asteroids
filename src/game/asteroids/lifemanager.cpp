#include "lifemanager.h"

LifeManager::LifeManager(ModelFactory* _factory){
    factory = _factory;

    lastPosition = QVector2D(-1.0, 0.93);
}

LifeManager::~LifeManager(){}

void LifeManager::IncreaseLifeCount(){
    lastPosition =QVector2D(lastPosition.x()+0.05, lastPosition.y());

    float size = Physics::shipLifeSize;
    auto ship = factory->GetScaledShipInstance(size);
    ship->atualPoint = lastPosition;
    ships.push_back(ship);
}

void LifeManager::DecreaseLifeCount(){
    lastPosition =QVector2D(lastPosition.x()-0.05, lastPosition.y());

    ships.pop_back();
}

void LifeManager::SetLifeCount(int newCount){
    ships.clear();
    lastPosition = QVector2D(-1.0, 0.93);

    for(int i =0; i<newCount; i++)
        IncreaseLifeCount();
}

int LifeManager::GetLifeCount(){
    return ships.size();
}

bool LifeManager::IsZero(){
    return ships.size() == 0;
}
