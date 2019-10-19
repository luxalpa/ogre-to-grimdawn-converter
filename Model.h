#ifndef MODEL_H
#define MODEL_H

#include <QList>
#include <QString>
#include <QMap>
#include <QtGui/QVector3D>
#include <QtGui/QMatrix4x4>
#include "Skeleton.h"

class QDomElement;
class QDomDocument;
class LxStream;
class Skeleton;

struct Vector3 {
    double x, y, z;
};

struct Vector2 {
    double x, y;
};

struct Triangle {
    int v1;
    int v2;
    int v3;
};

struct Weight {
    int boneID;
    float weight;
};

struct WeightEntry {
    QList<Weight> weights;
};

struct Geometry {
    QList<QVector3D> vertexPositions;
    QList<QVector3D> vertexNormals;
    QList<Vector2> UVs;
    QList<WeightEntry> vertexWeights;
};

class Model
{
public:
    Model();
    static Model fromFile(QString filename);
    static Model dummy();
    void exportMDL(QString filename);
    void addSkeleton(Skeleton skeleton);

private:
    static Geometry getGeometry(QDomElement mesh);
    static QList<Triangle> getFaces(QDomElement mesh);
    static QList<WeightEntry> getWeights(QDomElement mesh);

    void writeHeader(LxStream &stream);
    void writeShaderParams(LxStream &stream);
    void writeMesh(LxStream &stream, QMap<int, int> boneConv);
    QMap<int, int> writeBones(LxStream &stream);
    void addPseudoBone(QString name, QString parent);

    Geometry geometry;
    QList<Triangle> faces;
    Skeleton skeleton;
};


#endif // MODEL_H
