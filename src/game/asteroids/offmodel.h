#ifndef OFFMODEL_H
#define OFFMODEL_H

#include <QtOpenGL>
#include <QTextStream>
#include <QFile>
#include <fstream>
#include <limits>
#include <iostream>
#include <memory>

class OffModel {
public:
    OffModel(QString fileName);
    ~OffModel();

    std::unique_ptr<QVector4D[]> vertices;
    std::unique_ptr<unsigned int[]> indices;

    unsigned int numVertices;
    unsigned int numFaces;

    QVector3D midPoint;
    double invDiag;

    void drawModel(float angle, float scale);

private:
    void readOFFFile(const QString& fileName);
};

#endif // OFFMODEL_H
