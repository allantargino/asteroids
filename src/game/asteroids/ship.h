#ifndef SHIP_H
#define SHIP_H

#include "model.h"

class Ship : public Model {
public:
    Ship(QOpenGLWidget* _glWidget, std::shared_ptr<OffModel> _offModel, float _scale, QString _vertexShaderFile, QString _fragmentShaderFile, QVector3D _initialPosition);
    ~Ship();

    void MoveLeft();
    void MoveRight();
    void MoveUp();
};

#endif // SHIP_H
