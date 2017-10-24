#ifndef GUNSHOT_H
#define GUNSHOT_H

#include "model.h"

class Gunshot : public Model {
public:
    Gunshot(QOpenGLWidget* _glWidget, std::shared_ptr<OffModel> _offModel, float _scale, QString _vertexShaderFile, QString _fragmentShaderFile, QVector3D _initialPosition);
    ~Gunshot();
};

#endif // GUNSHOT_H
