#include "ship.h"

Ship::Ship(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, float _scale, QString _vertexShaderFile, QString _fragmentShaderFile, QVector3D _initialPosition): Model::Model(_glWidget, _offModel, _scale, _vertexShaderFile, _fragmentShaderFile, _initialPosition)
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
