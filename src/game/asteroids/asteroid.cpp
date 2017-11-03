#include "asteroid.h"

Asteroid::Asteroid(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, GLuint _shaderProgram, float _scale, QVector3D _initialPosition): Model::Model(_glWidget, _offModel, _shaderProgram, _scale, _initialPosition)
{
    float factor = Physics::GetRandomFactor(Physics::asteroidHighFactor, Physics::asteroidLowFactor);
    this->speed  = Physics::asteroidMovimentFactor * factor;
    this->isFragment = false;
}
Asteroid::~Asteroid()
{

}
