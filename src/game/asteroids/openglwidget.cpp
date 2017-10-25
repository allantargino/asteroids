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

    currentScore = 0;
    topPoints=0;
    level=0;
    tempTime = 0.0f;

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

    currentScore = 0;
    emit updateCurrentScore(currentScore);

    level = 1;
    emit updateLevel(QString("Fase: %1").arg(level));

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

        //shotPlayer->play();
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

    //Gunshots Colisions and Limits
    QHashIterator<QString, std::shared_ptr<Gunshot>> i(gunshots);
    while (i.hasNext()) {
        i.next();
        if(i.value())
        {
            auto gunshot = i.value();

            //Position
            gunshot->currentPosition = Physics::GetNextLinearMoviment
                    (
                        gunshot->currentPosition.x(),
                        gunshot->currentPosition.y(),
                        gunshot->angle,
                        Physics::gunshotAngleCorrection,
                        Physics::gunshotMovimentFactor * elapsedTime
                     );

            //Color
            float dist = (gunshot->currentPosition.distanceToPoint(gunshot->initialPosition))/2.0f;
            gunshot->color = 1.0 - dist;


            //Limits:
            if(qAbs(gunshot->currentPosition.x())>1.2 || qAbs(gunshot->currentPosition.y()) >1.2){
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

    //Asteroids Colisions and Limits
    QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
    while (i_ast.hasNext()) {
        i_ast.next();
        if(i_ast.value())
        {
            auto asteroid = i_ast.value();

            asteroid->currentPosition = Physics::GetNextLinearMoviment
                    (
                        asteroid->currentPosition.x(),
                        asteroid->currentPosition.y(),
                        asteroid->angle,
                        Physics::asteroidAngleCorrection,
                        Physics::asteroidMovimentFactor * elapsedTime
                     );

            //Limits:
            if(qAbs(asteroid->currentPosition.x())>1.4 || qAbs(asteroid->currentPosition.y()) >1.4){
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
                        setGameOver();
                        update();
                        return;
                    }
                }
            }
        }
    }

    //New Asteroids
    tempTime += elapsedTime;
    float asteroidTime = 2.0f / level;
    float launchTime = tempTime / asteroidTime;
    if(launchTime > 1){
        tempTime = 0;
        auto asteroid = factory->GetAsteroidInstance();
        asteroids[asteroid->id] = asteroid;
    }

    update();
}

void OpenGLWidget::increasePlayerScore(){
    //Current Score
    currentScore++;
    emit updateCurrentScore(currentScore);

    //Top Score
    if(currentScore>topPoints){
        topPoints = currentScore;
        emit updateTopPoints(topPoints);
    }

    //Level
    level = currentScore / 10 + 1;
    emit updateLevel(QString("Fase: %1").arg(level));
}

void OpenGLWidget::setGameOver(){
    playing = false;
    emit updateGameText(QString("GAME OVER"));
    emit updateButtonEnable(true);
    emit updateLevel("");
}
