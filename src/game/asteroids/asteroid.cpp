#include "asteroid.h"

Asteroid::Asteroid(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, GLuint _shaderProgram, float _scale, QVector3D _initialPosition): Model::Model(_glWidget, _offModel, _shaderProgram, _scale, _initialPosition)
{
    float HI = 1.3f;
    float LO = 0.7f;
    float factor = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

    this->speed  = Physics::asteroidMovimentFactor * factor;
}
Asteroid::~Asteroid()
{

}
