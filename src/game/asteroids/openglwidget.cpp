#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    angle=0.0;


    QString shipFile = "C:\\Repos\\asteroids\\src\\models\\ship.off";
    shipOffModel = std::make_shared<OffModel>(shipFile);
}
void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug("OpenGL version : %s", glGetString(GL_VERSION));
    qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!ship)
        return;

    //Nave do jogador
    ship->drawModel(angle, 0.1);

    //gunshot->drawModel(angle, 0.05);

    //Tiros do jogador
    //gunshot->drawModel(angle, X, Y, Z);

    //asteroid->drawModel(angle, x, y, z);
    //guardar info de radius e center(x,y,z)
}

void OpenGLWidget::loadSampleModel()
{
    //QString shipFile = "C:\\Repos\\asteroids\\src\\models\\ship.off";

    ship = std::make_shared<Ship>(this, shipOffModel);
    ship->Create();
    //ship->readOFFFile(shipFile);

    //QString gunshotFile = "C:\\Repos\\asteroids\\src\\models\\sphere.off";

   // gunshot = std::make_shared<Gunshot>(this);
    //gunshot->readOFFFile(gunshotFile);

    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        angle+=5.0;
        qDebug("Angle: %f", angle);
        break;
    case Qt::Key_Right:
        angle-=5.0;
        qDebug("Angle: %f", angle);
        break;
    case Qt::Key_Up:
        float xPos, yPos;
        xPos= ship->atualPoint.x() + 0.05*cos((angle + 90)* (3.1416/180));
        yPos= ship->atualPoint.y() + 0.05*sin((angle + 90)* (3.1416/180));
        ship->atualPoint = QVector3D(xPos, yPos, 0);
        break;
    case Qt::Key_Space:
        break;
    case Qt::Key_Escape:
        qApp->quit();
        break;
    default:
        break;
    }
    update();
}
