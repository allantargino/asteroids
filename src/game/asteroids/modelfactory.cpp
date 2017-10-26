#include "modelfactory.h"

ModelFactory::ModelFactory(QOpenGLWidget* _glWidget)
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

ModelFactory::~ModelFactory(){
    destroyShaders();
}


std::shared_ptr<Ship> ModelFactory::GetShipInstance(){
    float size =  Physics::shipSize;
    return GetScaledShipInstance(size);
}

std::shared_ptr<Ship> ModelFactory::GetScaledShipInstance(float size){
    QString vertexShaderFile(":/shaders/vshader_default.glsl");
    QString fragmentShaderFile(":/shaders/fshader_default.glsl");


    QVector3D position = QVector3D(0,0,0);
    std::shared_ptr<Ship> ship = std::make_shared<Ship>(glWidget, shipOffModel, shaderProgramDefault, size, position);
    ship->Create();

    ship->id = QUuid::createUuid().toString();

    return ship;
}

std::shared_ptr<Gunshot> ModelFactory::GetGunshotInstance(Ship* ship){
    if(!ship || GunshotQueue.isEmpty())
        return nullptr;

    QVector3D position = Physics::GetNextLinearMoviment
            (
                ship->currentPosition.x(),
                ship->currentPosition.y(),
                ship->angle,
                Physics::shipAngleCorrection,
                Physics::shipMovimentFactor
             );

    auto gunshot = GunshotQueue.dequeue();

    gunshot->currentPosition = position;
    gunshot->angle = ship->angle;

    return gunshot;
}

std::shared_ptr<Asteroid> ModelFactory::GetAsteroidInstance(){
    if(AsteroidQueue.isEmpty())
        return nullptr;

    auto asteroid = AsteroidQueue.dequeue();
    asteroid->currentPosition = asteroid->initialPosition;

    return asteroid;
}


void ModelFactory::RemoveAsteroidInstance(std::shared_ptr<Asteroid> asteroid){
    AsteroidQueue.enqueue(asteroid);
}

void ModelFactory::RemoveGunshotInstance(std::shared_ptr<Gunshot> gunshot){
    GunshotQueue.enqueue(gunshot);
}


void ModelFactory::LoadInstances(){   
    if(!isInitialized){
        initializeOpenGLFunctions();
        shaderProgramDefault =  createShaders();

        LoadAsteroidInstances();
        LoadGunshotInstances();
        isInitialized=true;
    }
}

void ModelFactory::LoadAsteroidInstances(){
    for (int i = 0; i < 100; ++i) {
        auto asteroid = CreateAsteroidInstance();
        AsteroidQueue.enqueue(asteroid);
    }
}

void ModelFactory::LoadGunshotInstances(){
    for (int i = 0; i < 100; ++i) {
        auto gunshot = CreateGunshotInstance();
        GunshotQueue.enqueue(gunshot);
    }
}


std::shared_ptr<Asteroid> ModelFactory::CreateAsteroidInstance(){
    QString vertexShaderFile(":/shaders/vshader_default.glsl");
    QString fragmentShaderFile(":/shaders/fshader_default.glsl");

    float HI = 2.0f;
    float LO = 0.5f;
    float factor = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    float size = Physics::asteroidMSize * factor;

    int choice = qrand() % 2;
    int AbsSignalChoice = qPow(-1, (qrand() % 2));
    int HVChoise = qPow(-1, (qrand() % 2));
    int AngleSignalChoice = qPow(-1, (qrand() % 2));

    float Var = 1.2;
    float absChoice = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/Var));
    float Ang = 45.0;
    float angleChoice = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/Ang));

    QVector2D initPoint;
    if(choice == 0){ // y fixed
        initPoint = QVector2D(absChoice* AbsSignalChoice, Var*HVChoise);
    }else{ // x fixed
        initPoint = QVector2D(Var*HVChoise,absChoice * AbsSignalChoice);
    }

    QVector2D initVector = initPoint.normalized();
    float angle;
    //Angle definition pointing to origin
    if(initVector.y()>=0){
        QVector2D refVector(1,0);
        float dot = QVector2D::dotProduct(refVector,initVector);
        angle = qRadiansToDegrees(qAcos(dot)) + 180;
    }else{
        QVector2D refVector(-1,0);
        float dot = QVector2D::dotProduct(refVector,initVector);
        angle = qRadiansToDegrees(qAcos(dot));
    }
    //Random angle
    angle += angleChoice * AngleSignalChoice;

    auto asteroid = std::make_shared<Asteroid>(glWidget, asteroidOffModel, shaderProgramDefault, size, initPoint);
    asteroid->Create();

    asteroid->initialPosition = initPoint;
    asteroid->currentPosition = initPoint;
    asteroid->angle = angle;
    asteroid->color = 0.5f;

    asteroid->id = QUuid::createUuid().toString();

    return asteroid;
}

std::shared_ptr<Gunshot> ModelFactory::CreateGunshotInstance(){
    QString vertexShaderFile(":/shaders/vshader_energy.glsl");
    QString fragmentShaderFile(":/shaders/fshader_default.glsl");

    float size =  Physics::gunshotSize;
    QVector3D position(0.0f,0.0f,0.0f);

    std::shared_ptr<Gunshot> gunshot = std::make_shared<Gunshot>(glWidget, gunshotOffModel, shaderProgramDefault, size, position);
    gunshot->Create();

    gunshot->id = QUuid::createUuid().toString();

    return gunshot;
}


GLuint ModelFactory::createShaders()
{
    // makeCurrent ();
    destroyShaders();

    QString vertexShaderFile(":/shaders/vshader_default.glsl");
    QString fragmentShaderFile(":/shaders/fshader_default.glsl");

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
    const GLchar* source = stdStringVs.c_str();
    glShaderSource(vertexShader, 1, &source, 0);
    // Compile the vertex shader
    glCompileShader(vertexShader);
    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
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
    if (isCompiled == GL_FALSE) {
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
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
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
