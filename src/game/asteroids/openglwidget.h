#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QtMath>
#include <QMediaPlayer>
#include <QHash>
#include "memory.h"
#include "ship.h"
#include "gunshot.h"
#include "offmodel.h"
#include "modelfactory.h"
#include "physics.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT

public:
    //OpenGL:
    OpenGLWidget(QWidget* parent = 0);
    //Models:
    std::shared_ptr<Ship> ship = nullptr;
    QHash<QString, std::shared_ptr<Gunshot>> gunshots;
    //Model Factory:
    std::unique_ptr<ModelFactory> factory = nullptr;
    //Animation:
    QTimer timer;
    QTime time;
    //Sounds:
    QMediaPlayer* player;

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent* event);

public slots:
    void loadSampleModel();
    void animate();
};
#endif // OPENGLWIDGET_H
