#ifndef SKELETON_H
#define SKELETON_H

#include <QString>
#include <QVector3D>
#include <QMatrix4x4>
#include <QMap>

class QDomElement;
class QDomDocument;

struct Bone {
    QString name;
    QList<QString> children;
    QString parent;
    QVector3D position;
    QQuaternion rotation;
    int id;
};

class Skeleton
{
public:
    Skeleton();
    int numBones() const;
    Bone bone(QString name) const;
    static Skeleton fromFile(QString filename, bool withPrefix = true);
    static Skeleton fromDocument(QDomDocument doc, bool withPrefix = true);
    QMap<QString, Bone> bones;

private:
    static QMap<QString, Bone> getBoneInfo(QDomElement bones);
};

#endif // SKELETON_H
