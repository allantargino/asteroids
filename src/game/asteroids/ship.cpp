#include "ship.h"

Ship::Ship(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, float _scale): Model::Model(_glWidget, _offModel, _scale)
{
    this->hitBoxRadius = (this->offModel->invDiag*scale)/2;
}
Ship::~Ship()
{
}

void Ship::MoveLeft(){
    if(this)
        this->angle+= Physics::shipMoveAngle;
}

void Ship::MoveRight(){
    if(this)
        this->angle-= Physics::shipMoveAngle;
}

void Ship::MoveUp(){
    if(this){
        float x= this->atualPoint.x() + 0.05*cos((this->angle + 90)* (3.1416/180));
        float y= this->atualPoint.y() + 0.05*sin((this->angle + 90)* (3.1416/180));
        this->atualPoint = QVector3D(x, y, 0);
    }
}
