#include "modelfactory.h"

ModelFactory::ModelFactory(QOpenGLWidget *_glWidget)
{
    glWidget = _glWidget;

    QString shipFile = "..\\..\\models\\ship.off";
    shipOffModel = std::make_shared<OffModel>(shipFile);

    QString gunshotFile = "..\\..\\models\\sphere.off";
    gunshotOffModel = std::make_shared<OffModel>(gunshotFile);

    QString asteroidFile = "..\\..\\models\\sphere.off";
    asteroidOffModel = std::make_shared<OffModel>(asteroidFile);

    glWidget->makeCurrent();
}

ModelFactory::~ModelFactory()
{
    destroyShaders();
}

std::shared_ptr<Ship> ModelFactory::GetShipInstance()
{
    float size = Physics::shipSize;
    return GetScaledShipInstance(size);
}

std::shared_ptr<Ship> ModelFactory::GetScaledShipInstance(float size)
{
    QVector3D position = QVector3D(0, 0, 0);
    std::shared_ptr<Ship> ship = std::make_shared<Ship>(glWidget, shipOffModel, shaderProgramDefault, size, position);
    ship->Create();

    ship->id = QUuid::createUuid().toString();

    return ship;
}

std::shared_ptr<Gunshot> ModelFactory::GetGunshotInstance(Ship *ship)
{
    if (!ship || GunshotQueue.isEmpty())
        return nullptr;

    QVector3D position = Physics::GetNextLinearMoviment(
        ship->currentPosition.x(),
        ship->currentPosition.y(),
        ship->angle,
        Physics::shipAngleCorrection,
        Physics::shipMovimentFactor);

    auto gunshot = GunshotQueue.dequeue();

    gunshot->initialPosition = position;
    gunshot->currentPosition = position;
    gunshot->angle = ship->angle;

    return gunshot;
}

std::shared_ptr<Asteroid> ModelFactory::GetAsteroidInstance()
{
    if (AsteroidQueue.isEmpty())
        return nullptr;

    auto asteroid = AsteroidQueue.dequeue();
    asteroid->currentPosition = asteroid->initialPosition;

    asteroid->isFragment = false;

    return asteroid;
}

std::vector<std::shared_ptr<Asteroid>> ModelFactory::GetFragmentInstance(const QVector3D &initPosition, float fatherSize)
{

    std::vector<std::shared_ptr<Asteroid>> fragments;

    //Number of fragments
    int n = (qrand() % Physics::fragmentQuantityAdditionalMax) + Physics::fragmentQuantityInitial; //From 3 to 6 [3, 4, 5, 6]

    //Angle
    float angle = Physics::GetRandomFactor(Physics::fragmentAngleHighFactor, Physics::fragmentAngleLowFactor);

    //Step
    float step = 360.0f / n;

    //Size
    float size = fatherSize / n;

    for (int i = 0; i < n; ++i)
    {
        auto asteroid = AsteroidQueue.dequeue();
        asteroid->isFragment = true;

        //Position
        asteroid->initialPosition = initPosition;
        asteroid->currentPosition = initPosition;

        asteroid->angle = angle + step * i;

        asteroid->scale = size;
        asteroid->speed = Physics::asteroidMovimentFactor * Physics::fragmentBooster;

        fragments.push_back(asteroid);
    }

    return fragments;
}

void ModelFactory::RemoveAsteroidInstance(std::shared_ptr<Asteroid> asteroid)
{
    AsteroidQueue.enqueue(asteroid);
}

void ModelFactory::RemoveGunshotInstance(std::shared_ptr<Gunshot> gunshot)
{
    GunshotQueue.enqueue(gunshot);
}

void ModelFactory::LoadInstances()
{
    if (!isInitialized)
    {
        initializeOpenGLFunctions();

        //Compile shaders:
        QString vertexShaderFileDefault(":/shaders/vshader_default.glsl");
        QString vertexShaderFileEnergy(":/shaders/vshader_energy.glsl");
        QString fragmentShaderFileDefault(":/shaders/fshader_default.glsl");

        shaderProgramDefault = createShaders(vertexShaderFileEnergy, fragmentShaderFileDefault);
        //shaderProgramEnergy = createShaders(vertexShaderFileEnergy, fragmentShaderFileDefault);

        LoadAsteroidInstances();
        LoadGunshotInstances();

        isInitialized = true;
    }
}

void ModelFactory::LoadAsteroidInstances()
{
    for (int i = 0; i < Physics::factoryAsteroidQuantity; ++i)
    {
        auto asteroid = CreateAsteroidInstance();
        AsteroidQueue.enqueue(asteroid);
    }
}

void ModelFactory::LoadGunshotInstances()
{
    for (int i = 0; i < Physics::factoryGunshotQuantity; ++i)
    {
        auto gunshot = CreateGunshotInstance();
        GunshotQueue.enqueue(gunshot);
    }
}

std::shared_ptr<Asteroid> ModelFactory::CreateAsteroidInstance()
{
    float factor = Physics::GetRandomFactor(Physics::asteroidSizeHighFactor, Physics::asteroidSizeLowFactor);
    float size = Physics::asteroidSize * factor;

    int choice = qrand() % 2;
    int AbsSignalChoice = qPow(-1, (qrand() % 2));
    int HVChoise = qPow(-1, (qrand() % 2));
    int AngleSignalChoice = qPow(-1, (qrand() % 2));

    float Var = Physics::factoryAsteroidInstancePosition;
    float absChoice = Physics::GetRandomFactor(Var, 0.0f);
    float Ang = Physics::factoryAsteroidInstanceAngle;
    float angleChoice = Physics::GetRandomFactor(Ang, 0.0f);

    QVector2D initPoint;
    if (choice == 0)
    { // y fixed
        initPoint = QVector2D(absChoice * AbsSignalChoice, Var * HVChoise);
    }
    else
    { // x fixed
        initPoint = QVector2D(Var * HVChoise, absChoice * AbsSignalChoice);
    }

    QVector2D initVector = initPoint.normalized();
    float angle;
    //Angle definition pointing to origin
    if (initVector.y() >= 0)
    {
        QVector2D refVector(1, 0);
        float dot = QVector2D::dotProduct(refVector, initVector);
        angle = qRadiansToDegrees(qAcos(dot)) + 180;
    }
    else
    {
        QVector2D refVector(-1, 0);
        float dot = QVector2D::dotProduct(refVector, initVector);
        angle = qRadiansToDegrees(qAcos(dot));
    }
    //Random angle
    angle += angleChoice * AngleSignalChoice;

    auto asteroid = std::make_shared<Asteroid>(glWidget, asteroidOffModel, shaderProgramDefault, size, initPoint);
    asteroid->Create();

    asteroid->initialPosition = initPoint;
    asteroid->currentPosition = initPoint;
    asteroid->angle = angle;

    asteroid->id = QUuid::createUuid().toString();

    return asteroid;
}

std::shared_ptr<Gunshot> ModelFactory::CreateGunshotInstance()
{
    float size = Physics::gunshotSize;
    QVector3D position(0.0f, 0.0f, 0.0f);

    std::shared_ptr<Gunshot> gunshot = std::make_shared<Gunshot>(glWidget, gunshotOffModel, shaderProgramDefault, size, position);
    gunshot->Create();

    gunshot->id = QUuid::createUuid().toString();

    return gunshot;
}

GLuint ModelFactory::createShaders(const QString &vertexShaderFile, const QString &fragmentShaderFile)
{
    // makeCurrent ();
    destroyShaders();

    QFile vs(vertexShaderFile);
    QFile fs(fragmentShaderFile);

    vs.open(QFile::ReadOnly | QFile::Text);
    fs.open(QFile::ReadOnly | QFile::Text);

    QTextStream streamVs(&vs), streamFs(&fs);

    QString qtStringVs = streamVs.readAll();
    QString qtStringFs = streamFs.readAll();

    std::string stdStringVs = qtStringVs.toStdString();
    std::string stdStringFs = qtStringFs.toStdString();

    // Create an empty vertex shader handle
    GLuint vertexShader = 0;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Send the vertex shader source code to GL
    const GLchar *source = stdStringVs.c_str();
    glShaderSource(vertexShader, 1, &source, 0);
    // Compile the vertex shader
    glCompileShader(vertexShader);
    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        qDebug("%s", &infoLog[0]);
        glDeleteShader(vertexShader);
        return 0;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = 0;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Send the fragment shader source code to GL
    // Note that std :: string ’s . c_str is NULL character terminated.
    source = stdStringFs.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);
    // Compile the fragment shader
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength,
                           &infoLog[0]);
        qDebug("%s", &infoLog[0]);
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        return 0;
    }

    GLuint shaderProgram = glCreateProgram();
    // Attach our shaders to our program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Link our program
    glLinkProgram(shaderProgram);
    // Note the different functions here : glGetProgram * instead of glGetShader *.
    GLint isLinked = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength,
                            &infoLog[0]);
        qDebug("%s", &infoLog[0]);
        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    vs.close();
    fs.close();

    return shaderProgram;
}

void ModelFactory::destroyShaders()
{
    glDeleteProgram(shaderProgramDefault);
}
