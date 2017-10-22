#include "ship.h"

Ship::Ship(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel): Model::Model(_glWidget, _offModel)
{
}
Ship::~Ship()
{
}

void Ship::MoveLeft(){
    this->angle+= Physics::shipMoveAngle;
}

void Ship::MoveRight(){
    this->angle-= Physics::shipMoveAngle;
}

void Ship::MoveUp(){
    float xPos, yPos;
    xPos= this->atualPoint.x() + 0.05*cos((this->angle + 90)* (3.1416/180));
    yPos= this->atualPoint.y() + 0.05*sin((this->angle + 90)* (3.1416/180));
    this->atualPoint = QVector3D(xPos, yPos, 0);
}
