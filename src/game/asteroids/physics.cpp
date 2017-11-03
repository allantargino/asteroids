#include "physics.h"

Physics::Physics(){
}

Physics::~Physics(){}


QVector3D Physics::GetNextLinearMoviment(float currentX, float currentY, float currentAngle, float angleCorrection, float movimentFactor){
    float x = currentX + movimentFactor*cos(qDegreesToRadians((currentAngle + angleCorrection)));
    float y = currentY + movimentFactor*sin(qDegreesToRadians((currentAngle + angleCorrection)));
    return QVector3D(x, y, 0);
}

float Physics::GetRandomFactor(float high, float low){
    float HI = high;
    float LO = low;
    float factor = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    return factor;
}
