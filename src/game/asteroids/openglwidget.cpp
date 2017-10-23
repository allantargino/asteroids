#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent): QOpenGLWidget(parent)
{
    factory = std::make_unique<ModelFactory>(this);

    lifeManager = std::make_unique<LifeManager>(factory.get());

    shipPlayer = new QMediaPlayer;
    shipPlayer->setMedia(QUrl::fromLocalFile("C:\\Repos\\asteroids\\src\\sounds\\bangLarge.wav"));
    shipPlayer->setVolume(100);

    shotPlayer = new QMediaPlayer;
    shotPlayer->setMedia(QUrl::fromLocalFile("C:\\Repos\\asteroids\\src\\sounds\\fire.wav"));
    shotPlayer->setVolume(100);

    asteroidPlayer = new QMediaPlayer;
    asteroidPlayer->setMedia(QUrl::fromLocalFile("C:\\Repos\\asteroids\\src\\sounds\\bangSmall.wav"));
    asteroidPlayer->setVolume(100);

    currentPoints = 0;
    topPoints=0;

    playing = false;
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

    if(!playing)
        return;

    //Nave do jogador
    if (!ship)
        return;
    ship->drawModel();

    //Vidas do jogador
    for (std::shared_ptr<Ship> &life : lifeManager->ships)
    {
        if(life)
            life->drawModel();
    }

    //Tiros da nave
    QHashIterator<QString, std::shared_ptr<Gunshot>> i(gunshots);
    while (i.hasNext()) {
        i.next();
        if(i.value())
            i.value()->drawModel();
    }

    //Asteroids
    QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
    while (i_ast.hasNext()) {
        i_ast.next();
        if(i_ast.value())
            i_ast.value()->drawModel();
    }
}

void OpenGLWidget::startGame()
{
    emit updateButtonEnable(false);
    emit updateGameText(QString(""));

    currentPoints = 0;
    emit updateCurrentPoints(currentPoints);

    lifeManager->SetLifeCount(5);

    ship = factory->GetShipInstance();

    playing = true;

    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        ship->MoveLeft();
        break;
    case Qt::Key_Right:
        ship->MoveRight();
        break;
    case Qt::Key_Up:
        ship->MoveUp();
        break;
    case Qt::Key_Space:
    {
        if(!playing)
            return;

        auto gunshot = factory->GetGunshotInstance(ship.get());
        gunshots[gunshot->id] = gunshot;

        shotPlayer->play();
    }
        break;
    case Qt::Key_A:
    {
        auto asteroid = factory->GetAsteroidInstance();
        asteroids[asteroid->id] = asteroid;
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
    if(!playing)
        return;

    float elapsedTime = time.restart() / 1000.0f;

    //Gunshots
    QHashIterator<QString, std::shared_ptr<Gunshot>> i(gunshots);
    while (i.hasNext()) {
        i.next();
        if(i.value())
        {
            auto gunshot = i.value();

            float xPos, yPos;
            xPos= gunshot->atualPoint.x() + elapsedTime * 0.8 * cos((gunshot->angle + 90)* (3.1416/180));
            yPos= gunshot->atualPoint.y() + elapsedTime * 0.8 * sin((gunshot->angle + 90)* (3.1416/180));
            gunshot->atualPoint =  QVector3D(xPos, yPos, 0);


            //Limits:
            if(qAbs(gunshot->atualPoint.x())>1.2 || qAbs(gunshot->atualPoint.y()) >1.2){
                gunshots.remove(gunshot->id);
                gunshot.reset();
            }else{
                //Points:
                QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
                while (i_ast.hasNext()) {
                    i_ast.next();
                    if(i_ast.value())
                    {
                        auto asteroid = i_ast.value();
                        if(gunshot->CalculateColision(asteroid.get())){
                            qDebug("Colision Detected!!");
                            asteroidPlayer->play();
                            increasePlayerScore();
                            gunshots.remove(gunshot->id);
                            gunshot.reset();
                            asteroids.remove(asteroid->id);
                            asteroid.reset();
                            break;
                        }
                    }
                }
            }
        }
    }

    //Asteroids
    QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
    while (i_ast.hasNext()) {
        i_ast.next();
        if(i_ast.value())
        {
            auto asteroid = i_ast.value();

            float xPos, yPos;
            xPos= asteroid->atualPoint.x() + elapsedTime * 0.5 * cos((asteroid->angle)* (3.1416/180));
            yPos= asteroid->atualPoint.y() + elapsedTime * 0.5 * sin((asteroid->angle)* (3.1416/180));
            asteroid->atualPoint =  QVector3D(xPos, yPos, 0);

            //Limits:
            if(qAbs(asteroid->atualPoint.x())>1.4 || qAbs(asteroid->atualPoint.y()) >1.4){
                asteroids.remove(asteroid->id);
                asteroid.reset();
            }else{
                //Crash:
                if(ship->CalculateColision(asteroid.get())){
                    shipPlayer->play();

                    asteroids.remove(asteroid->id);
                    asteroid.reset();

                    lifeManager->DecreaseLifeCount();
                    if(lifeManager->IsZero()){
                        playing = false;
                        emit updateGameText(QString("GAME OVER"));
                        emit updateButtonEnable(true);
                        update();
                        return;
                    }
                }
            }
        }
    }

    update();
}

void OpenGLWidget::increasePlayerScore(){
    currentPoints++;
    emit updateCurrentPoints(currentPoints);
    if(currentPoints>topPoints){
        topPoints = currentPoints;
        emit updateTopPoints(topPoints);
    }
}
