#include "asteroid.h"

Asteroid::Asteroid(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, float _scale, QString _vertexShaderFile, QString _fragmentShaderFile, QVector3D _initialPosition): Model::Model(_glWidget, _offModel, _scale, _vertexShaderFile, _fragmentShaderFile, _initialPosition)
{

}
Asteroid::~Asteroid()
{

}
