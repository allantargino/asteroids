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
#include "asteroid.h"
#include "offmodel.h"
#include "modelfactory.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT

public:
    //OpenGL:
    OpenGLWidget(QWidget* parent = 0);
    //Models:
    std::shared_ptr<Ship> ship = nullptr;
    QHash<QString, std::shared_ptr<Gunshot>> gunshots;
    QHash<QString, std::shared_ptr<Asteroid>> asteroids;
    //Model Factory:
    std::unique_ptr<ModelFactory> factory = nullptr;
    //Animation:
    QTimer timer;
    QTime time;
    //Sounds:
    QMediaPlayer* shotPlayer;
    QMediaPlayer* asteroidPlayer;

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent* event);

private:
    int currentPoints;
    int topPoints;

    void increasePlayerScore();

public slots:
    void startGame();
    void animate();

signals:
    void updateCurrentPoints(int points);
    void updateTopPoints(int points);
};
#endif // OPENGLWIDGET_H
