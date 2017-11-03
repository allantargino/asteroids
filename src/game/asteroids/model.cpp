#include "model.h"

Model::Model(QOpenGLWidget* _glWidget,  std::shared_ptr<OffModel> _offModel, const GLuint &_shaderProgram, float _scale, const QVector3D &_initialPosition)
{
    glWidget = _glWidget;
    offModel = _offModel;
    shaderProgram = _shaderProgram;

    scale = _scale;

    this->hitBoxRadius = this->offModel->invDiag*scale;

    this->initialPosition = _initialPosition;
    this->currentPosition = QVector3D(0.0, 0.0, 0.0);

    this->angle=Physics::modelInitialAngle;
    this->color=Physics::modelInitialColor;

    glWidget->makeCurrent();

    initializeOpenGLFunctions();
}
Model::~Model()
{
    destroyVBOs();
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

void Model::drawModel()
{
    modelMatrix.setToIdentity(); //M=I
    modelMatrix.translate(currentPosition);
    modelMatrix.rotate(angle, 0.0, 0.0, 1.0); // Rotação apenas em Z (plano XY)
    modelMatrix.scale(offModel->invDiag*scale, offModel->invDiag*scale, offModel->invDiag*scale); //M=I*S
    modelMatrix.translate(-offModel->midPoint); //M=I*S*T

    glBindVertexArray(vao);
    glUseProgram(shaderProgram);

    GLuint locModelMatrix = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(locModelMatrix, 1, GL_FALSE, modelMatrix.data());

    GLuint locColor = glGetUniformLocation(shaderProgram, "color");
    glUniform1f(locColor, color);

    glDrawElements(GL_TRIANGLES, offModel->numFaces * 3, GL_UNSIGNED_INT, 0);
}

void Model::Create()
{
    createVBOs();
}

//Extra Functions:

bool Model::CalculateColision(Model* other)
{   
    float distance = this->currentPosition.distanceToPoint(other->currentPosition);
    return distance < (this->hitBoxRadius + other->hitBoxRadius);
}
