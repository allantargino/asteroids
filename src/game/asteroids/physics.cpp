#include "physics.h"

Physics::Physics(){
}

Physics::~Physics(){}

bool Physics::CalculateColision(Model &A, Model &B){
    float distance = A.atualPoint.distanceToPoint(B.atualPoint);
    return distance < (A.hitBoxRadius + B.hitBoxRadius);;
}
