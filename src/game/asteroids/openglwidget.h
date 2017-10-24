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
#include "lifemanager.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT

public:
    //OpenGL:
    OpenGLWidget(QWidget* parent = 0);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent* event);

private:
    //Models:
    std::shared_ptr<Ship> ship = nullptr;
    QHash<QString, std::shared_ptr<Gunshot>> gunshots;
    QHash<QString, std::shared_ptr<Asteroid>> asteroids;
    //Model Factory:
    std::unique_ptr<ModelFactory> factory = nullptr;
    //Life Manager:
    std::unique_ptr<LifeManager> lifeManager = nullptr;
    //Animation:
    QTimer timer;
    QTime time;
    //Sounds:
    QMediaPlayer* shipPlayer;
    QMediaPlayer* shotPlayer;
    QMediaPlayer* asteroidPlayer;
    //Control:
    bool playing;
    int currentScore;
    int topPoints;

    void increasePlayerScore();

public slots:
    void startGame();
    void animate();

signals:
    void updateCurrentScore(int points);
    void updateTopPoints(int points);
    void updateGameText(QString text);
    void updateButtonEnable(bool value);
};
#endif // OPENGLWIDGET_H
