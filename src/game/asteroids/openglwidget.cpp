#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    factory = std::make_unique<ModelFactory>(this);

    player = new QMediaPlayer;

    player->setMedia(QUrl::fromLocalFile("C:\\Repos\\asteroids\\src\\sounds\\fire.wav"));
    player->setVolume(100);
}
void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug("OpenGL version : %s", glGetString(GL_VERSION));
    qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);

    connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer.start(0);

    time.start();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Nave do jogador
    if (!ship)
        return;
    ship->drawModel(Physics::shipSize);

    //Tiros da nave
    QHashIterator<QString, std::shared_ptr<Gunshot>> i(gunshots);
    while (i.hasNext()) {
        i.next();
        if(i.value())
            i.value()->drawModel(Physics::gunshotSize);
    }

    //asteroid->drawModel(angle, x, y, z);
    //guardar info de radius e center(x,y,z)
}

void OpenGLWidget::loadSampleModel()
{
    ship = factory->GetShipInstance();

    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        ship->angle+=5.0;
        break;
    case Qt::Key_Right:
        ship->angle-=5.0;
        break;
    case Qt::Key_Up:
        float xPos, yPos;
        xPos= ship->atualPoint.x() + 0.05*cos((ship->angle + 90)* (3.1416/180));
        yPos= ship->atualPoint.y() + 0.05*sin((ship->angle + 90)* (3.1416/180));
        ship->atualPoint = QVector3D(xPos, yPos, 0);
        break;
    case Qt::Key_Space:
    {
        auto gunshot = factory->GetGunshotInstance(ship.get());
        gunshots[gunshot->id] = gunshot;

        player->play();
    }
        break;
    case Qt::Key_Escape:
        qApp->quit();
        break;
    default:
        break;
    }
    update();
}

void OpenGLWidget::animate()
{
    float elapsedTime = time.restart() / 1000.0f;

    QHashIterator<QString, std::shared_ptr<Gunshot>> i(gunshots);
    while (i.hasNext()) {
        i.next();
        if(i.value())
        {
            auto gunshot = i.value();

            float xPos, yPos;
            xPos= gunshot->atualPoint.x() + elapsedTime * 2 * cos((gunshot->angle + 90)* (3.1416/180));
            yPos= gunshot->atualPoint.y() + elapsedTime * 2 * sin((gunshot->angle + 90)* (3.1416/180));
            gunshot->atualPoint =  QVector3D(xPos, yPos, 0);

            //Limits:
            if(qAbs(gunshot->atualPoint.x())>1.2 || qAbs(gunshot->atualPoint.y()) >1.2){
                gunshots.remove(gunshot->id);
                gunshot.reset();
            }
        }
    }

    update();
}
