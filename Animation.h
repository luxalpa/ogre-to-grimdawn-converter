#ifndef ANIMATION_H
#define ANIMATION_H

#include <QString>
#include <QList>
#include <QVector3D>
#include <QQuaternion>
#include "Skeleton.h"

class LxStream;

struct Keyframe {
    float time;
    QVector3D translation;
    QQuaternion rotation;
};

struct Track {
    QString bone;
    QList<Keyframe> keyframes;

    Keyframe getKeyframeAt(float time);
};

struct CallbackPoint {
    CallbackPoint(QString name, int frame) {
        this->frame = frame;
        this->name = name;
    }

    QString name;
    int frame;
};

struct CreateEntity {
    CreateEntity(QString entity, QString attach, int frame) {
        this->frame = frame;
        this->attach = attach;
        this->entity = entity;
    }

    QString entity;
    QString attach;
    int frame;
};

struct ExtraData {
    QList<CallbackPoint> callbackPoints;
    QList<CreateEntity> createEntities;

    QString toString() const;
};

class Animation
{
public:
    Animation();
    static Animation fromFile(QString filename);
    static Animation dummy();
    void exportAnm(QString filepath);
    void applyBindPose(Skeleton sk);
    void setExtraData(ExtraData d);

private:
    void writeHeader(LxStream &stream);

    int fps;
    float length;

    QList<Track> tracks;

    // The skeletal position that this animation is based on
    // (which may be different from the bind position)
    Skeleton skeleton;
    ExtraData extraData;
};

#endif // ANIMATION_H
