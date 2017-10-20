#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include "memory.h"
#include "ship.h"
#include "gunshot.h"
#include "offmodel.h"
#include "modelfactory.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT

public:
    OpenGLWidget(QWidget* parent = 0);
    std::shared_ptr<Ship> ship = nullptr;
    std::shared_ptr<Ship> ship2 = nullptr;
    std::shared_ptr<Gunshot> gunshot = nullptr;

    std::shared_ptr<ModelFactory> factory = nullptr;

    float angle;

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent* event);

signals:
    void statusBarMessage(QString);

public slots:
    void loadSampleModel();
};
#endif // OPENGLWIDGET_H
