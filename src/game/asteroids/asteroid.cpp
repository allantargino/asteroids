#include "asteroid.h"

Asteroid::Asteroid(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, float _scale): Model::Model(_glWidget, _offModel, _scale)
{

}
Asteroid::~Asteroid()
{

}
