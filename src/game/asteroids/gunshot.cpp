#include "gunshot.h"

Gunshot::Gunshot(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, float _scale, QString _vertexShaderFile, QString _fragmentShaderFile): Model::Model(_glWidget, _offModel, _scale, _vertexShaderFile, _fragmentShaderFile)
{

}
Gunshot::~Gunshot()
{

}
