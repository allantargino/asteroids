#include "asteroid.h"

Asteroid::Asteroid(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, const GLuint &_shaderProgram, float _scale, const QVector3D &_initialPosition): Model::Model(_glWidget, _offModel, _shaderProgram, _scale, _initialPosition)
{
    float factor = Physics::GetRandomFactor(Physics::asteroidMovimentHighFactor, Physics::asteroidMovimentLowFactor);
    this->speed  = Physics::asteroidMovimentFactor * factor;
    this->isFragment = false;
}
Asteroid::~Asteroid()
{
}
