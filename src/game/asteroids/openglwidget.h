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

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT

public:
    OpenGLWidget(QWidget* parent = 0);
    std::shared_ptr<Ship> ship = nullptr;
    //std::shared_ptr<Gunshot> gunshot = nullptr;

    QHash<QString, std::shared_ptr<Gunshot>> gunshots;

    std::shared_ptr<ModelFactory> factory = nullptr;

    QTimer timer;
    QTime time;

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
