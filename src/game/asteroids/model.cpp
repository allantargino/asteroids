#include "model.h"

Model::Model(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, float _scale)
{
    offModel = _offModel;
    glWidget = _glWidget;
    scale = _scale;

    this->hitBoxRadius = this->offModel->invDiag*scale;
    this->atualPoint = QVector3D(0.0, 0.0, 0.0);
    this->angle=0.0;

    glWidget->makeCurrent();

    initializeOpenGLFunctions();
}
Model::~Model()
{
    destroyVBOs();
    destroyShaders();
}

void Model::destroyVBOs()
{
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboIndices);
    glDeleteVertexArrays(1, &vao);
    vboVertices = 0;
    vboIndices = 0;
    vao = 0;
}

void Model::destroyShaders()
{
    glDeleteProgram(shaderProgram);
}

void Model::createVBOs()
{
    glWidget->makeCurrent();
    destroyVBOs();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, offModel->numVertices * sizeof(QVector4D), offModel->vertices.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, offModel->numFaces * 3 * sizeof(
                                                             unsigned int),
        offModel->indices.get(), GL_STATIC_DRAW);
}

void Model::createShaders()
{
    // makeCurrent ();
    destroyShaders();
    QString vertexShaderFile(":/shaders/vshader1.glsl");
    QString fragmentShaderFile(":/shaders/fshader1.glsl");

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
        return;
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
        return;
    }

    shaderProgram = glCreateProgram();
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
        return;
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    vs.close();
    fs.close();
}

void Model::drawModel()
{
    modelMatrix.setToIdentity(); //M=I
    modelMatrix.translate(atualPoint);
    modelMatrix.rotate(angle, 0.0, 0.0, 1.0); // Rotação apenas em Z (plano XY)
    modelMatrix.scale(offModel->invDiag*scale, offModel->invDiag*scale, offModel->invDiag*scale); //M=I*S
    modelMatrix.translate(-offModel->midPoint); //M=I*S*T

    glBindVertexArray(vao);
    glUseProgram(shaderProgram);

    GLuint locModelMatrix = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(locModelMatrix, 1, GL_FALSE, modelMatrix.data());

    glDrawElements(GL_TRIANGLES, offModel->numFaces * 3, GL_UNSIGNED_INT, 0);
}

void Model::Create()
{
    createShaders();
    createVBOs();
}

//Extra Functions:

bool Model::CalculateColision(Model* other)
{   
    float distance = this->atualPoint.distanceToPoint(other->atualPoint);
    return distance < (this->hitBoxRadius + other->hitBoxRadius);
}
