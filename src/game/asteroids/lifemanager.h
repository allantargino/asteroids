#ifndef LIFEMANAGER_H
#define LIFEMANAGER_H

#include <vector>
#include "ship.h"
#include "physics.h"
#include "modelfactory.h"

class LifeManager
{
  public:
    LifeManager(ModelFactory *_factory);
    ~LifeManager();

    std::vector<std::shared_ptr<Ship>> ships;

    //Model Factory:
    ModelFactory *factory;

    void IncreaseLifeCount();
    void DecreaseLifeCount();

    void SetLifeCount(int count);
    int GetLifeCount();
    bool IsZero();

  private:
    int count;
    QVector2D lastPosition;
};

#endif // LIFEMANAGER_H
