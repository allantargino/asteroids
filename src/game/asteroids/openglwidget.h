#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QtMath>
#include <QMediaPlayer>
#include <QSoundEffect>
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
    ~OpenGLWidget();

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
    int level;
    float tempTime;

    //Animations:
    void checkGunshotsColisions(float elapsedTime);
    void checkAsteroidsColisions(float elapsedTime);
    void insertNewAsteroids(float elapsedTime);

    void removeAllAsteroids();
    void removeAllGunshots();

    void increasePlayerScore();
    void setGameOver();

    void triggerGunshot();

    //Init:
    QMediaPlayer* getMediaPlayer(const QString &file);

public slots:
    void startGame();
    void animate();

signals:
    void updateCurrentScore(int points);
    void updateTopPoints(int points);
    void updateGameText(const QString &text);
    void updateButtonEnable(bool value);
    void updateLevel(const QString &text);
};
#endif // OPENGLWIDGET_H
