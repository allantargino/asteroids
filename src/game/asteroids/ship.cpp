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
        this->currentPosition = Physics::GetNextLinearMoviment
                (
                    this->currentPosition.x(),
                    this->currentPosition.y(),
                    this->angle,
                    Physics::shipAngleCorrection,
                    Physics::shipMovimentFactor
                 );
    }
}
