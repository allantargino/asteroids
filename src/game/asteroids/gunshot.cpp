#include "gunshot.h"

Gunshot::Gunshot(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, GLuint _shaderProgram, float _scale, QVector3D _initialPosition): Model::Model(_glWidget, _offModel, _shaderProgram, _scale, _initialPosition)
{

}
Gunshot::~Gunshot()
{

}
