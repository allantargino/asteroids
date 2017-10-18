#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    angle=0.0;
    X=0.0;
    Y=0.0;
    Z=1.0;
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
    ship->drawModel(angle, X, Y, Z);

    //Tiros do jogador
    //gunshot->drawModel(angle, X, Y, Z);

    //asteroid->drawModel(angle, x, y, z);
    //guardar info de radius e center(x,y,z)
}

void OpenGLWidget::showFileOpenDialog()
{
    QByteArray fileFormat = "off ";
    QString fileName = QFileDialog::getOpenFileName(this,
        " Open File ", QDir::homePath(),
        QString("\%1 Files (*.\%2) ").arg(QString(fileFormat.toUpper())).arg(QString(fileFormat)), 0
#ifdef Q_OS_LINUX
        ,
        QFileDialog::DontUseNativeDialog
#endif
        );
    if (!fileName.isEmpty()) {
        model = std::make_shared<Model>(this);
        model->readOFFFile(fileName);
        emit statusBarMessage(QString("Vertices: \%1 , Faces : \%2").arg(model->numVertices).arg(model->numFaces));
    }
    update();
}

void OpenGLWidget::loadSampleModel()
{
    QString fileName = "C:\\Repos\\asteroids\\src\\models\\ship.off";

    ship = std::make_shared<Ship>(this);
    ship->readOFFFile(fileName);

    //gunshot = std::make_shared<Gunshot>(this);
    //gunshot->readOFFFile(fileName);

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
