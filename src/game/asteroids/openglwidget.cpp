#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent): QOpenGLWidget(parent)
{
    factory = std::make_unique<ModelFactory>(this);

    lifeManager = std::make_unique<LifeManager>(factory.get());

    shipPlayer = new QMediaPlayer;
    shipPlayer->setVolume(0);
    shipPlayer->setMedia(QUrl::fromLocalFile("..\\..\\sounds\\bangLarge.wav"));
    shipPlayer->play();

    shotPlayer = new QMediaPlayer;
    shotPlayer->setVolume(0);
    shotPlayer->setMedia(QUrl::fromLocalFile("..\\..\\sounds\\fire.wav"));
    shotPlayer->play();

    asteroidPlayer = new QMediaPlayer;
    asteroidPlayer->setVolume(0);
    asteroidPlayer->setMedia(QUrl::fromLocalFile("..\\..\\sounds\\bangSmall.wav"));
    asteroidPlayer->play();

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
    ship->drawModel();

    //Vidas do jogador
    for (std::shared_ptr<Ship> &life : lifeManager->ships)
        life->drawModel();

    //Tiros da nave
    QHashIterator<QString, std::shared_ptr<Gunshot>> i(gunshots);
    while (i.hasNext()) {
        i.next();
        i.value()->drawModel();
    }

    //Asteroids
    QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
    while (i_ast.hasNext()) {
        i_ast.next();
        i_ast.value()->drawModel();
    }
}

void OpenGLWidget::startGame()
{
    //Resource Pool
    factory->LoadInstances();

    //Audio enable
    shipPlayer->setVolume(100);
    shotPlayer->setVolume(100);
    asteroidPlayer->setVolume(100);

    //Enable state handling
    emit updateButtonEnable(false);
    emit updateGameText(QString(""));

    //Current score handling
    currentScore = 0;
    emit updateCurrentScore(currentScore);

    //Level handling
    level = 1;
    emit updateLevel(QString("Fase: %1").arg(level));

    //5 lifes
    lifeManager->SetLifeCount(5);

    //Create Ship
    ship = factory->GetShipInstance();

    //Game starts
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
        triggerGunshot();
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
    checkGunshotsColisions(elapsedTime);

    //Asteroids Colisions and Limits
    checkAsteroidsColisions(elapsedTime);

    //New Asteroids
    insertNewAsteroids(elapsedTime);

    update();
}

void OpenGLWidget::checkGunshotsColisions(float elapsedTime){
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
                //Score:
                QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
                while (i_ast.hasNext()) {
                    i_ast.next();
                    auto asteroid = i_ast.value();
                    if(asteroid)
                    {
                        if(gunshot->CalculateColision(asteroid.get())){
                            qDebug("Colision Detected!!");
                            asteroidPlayer->play();
                            increasePlayerScore();

                            gunshots.remove(gunshot->id);
                            factory->RemoveGunshotInstance(gunshot);

                            asteroids.remove(asteroid->id);
                            factory->RemoveAsteroidInstance(asteroid);

                            break;
                        }
                    }
                }
            }
        }
    }
}

void OpenGLWidget::checkAsteroidsColisions(float elapsedTime){
    QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
    while (i_ast.hasNext()) {
        i_ast.next();
        auto asteroid = i_ast.value();
        if(asteroid)
        {
            asteroid->color=1.0;

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
                    factory->RemoveAsteroidInstance(asteroid);

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
}

void OpenGLWidget::insertNewAsteroids(float elapsedTime){
    tempTime += elapsedTime;
    float asteroidTime = 2.0f / level;
    float launchTime = tempTime / asteroidTime;
    if(launchTime > 1){
        tempTime = 0;
        auto asteroid = factory->GetAsteroidInstance();
        asteroids[asteroid->id] = asteroid;
    }
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

    removeAllAsteroids();
    removeAllGunshots();
}

void OpenGLWidget::removeAllAsteroids(){
    QHashIterator<QString, std::shared_ptr<Asteroid>> i_ast(asteroids);
    while (i_ast.hasNext()) {
        i_ast.next();
        auto asteroid = i_ast.value();
        if(asteroid)
        {
            asteroids.remove(asteroid->id);
            factory->RemoveAsteroidInstance(asteroid);
        }
    }
}

void OpenGLWidget::removeAllGunshots(){
    QHashIterator<QString, std::shared_ptr<Gunshot>> i_ast(gunshots);
    while (i_ast.hasNext()) {
        i_ast.next();
        auto gunshot = i_ast.value();
        if(gunshot)
        {
            gunshots.remove(gunshot->id);
            factory->RemoveGunshotInstance(gunshot);
        }
    }
}

void OpenGLWidget::triggerGunshot(){
    if(!playing)
        return;

    auto gunshot = factory->GetGunshotInstance(ship.get());
    gunshots[gunshot->id] = gunshot;

    shotPlayer->play();
}
