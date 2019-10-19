#include "Animation.h"
#include <QDomDocument>
#include "Utils.h"
#include <QDebug>
#include "LxStream.h"
#include <QtMath>
#include <QQuaternion>

Animation::Animation() {

}

Animation Animation::fromFile(QString filepath) {
    Animation animation;
    animation.fps = 30;

    QDomDocument doc = Utils::readXMLFile(filepath);
    QDomElement an = doc.firstChildElement("skeleton")
            .firstChildElement("animations")
            .firstChildElement("animation");
    animation.length = an.attribute("length").toFloat();
    QDomNodeList trackList = an.firstChildElement("tracks").childNodes();
    for(int i = 0; i < trackList.length(); i++) {
        QDomElement trackElement = trackList.item(i).toElement();
        Track track;

        track.bone = trackElement.attribute("bone");

        if(track.bone != "root") {
            track.bone = "Dx_" + track.bone;
        }

        QDomNodeList keyframes = trackElement.firstChildElement("keyframes").childNodes();
        for(int j = 0; j < keyframes.length(); j++) {
            QDomElement keyframe = keyframes.item(j).toElement();
            Keyframe kf;
            kf.time = keyframe.attribute("time").toFloat();

            QDomElement tr = keyframe.firstChildElement("translate");
            kf.translation = {
                tr.attribute("x").toFloat(),
                tr.attribute("y").toFloat(),
                tr.attribute("z").toFloat(),
            };

            QDomElement rt = keyframe.firstChildElement("rotate");
            float angle = rt.attribute("angle").toFloat();

            QDomElement ax = rt.firstChildElement("axis");
            QVector3D vec = {
                ax.attribute("x").toFloat(),
                ax.attribute("y").toFloat(),
                ax.attribute("z").toFloat(),
            };

            kf.rotation = QQuaternion::fromAxisAndAngle(vec, qRadiansToDegrees(angle));

            track.keyframes.append(kf);
        }

        animation.tracks.append(track);
    }

    animation.skeleton = Skeleton::fromDocument(doc);

    return animation;
}

Animation Animation::dummy() {
    Animation animation;
    animation.fps = 1;
    animation.length = 3;

    Track track;
    track.bone = "Bone1";

    Keyframe kf;
    kf.time = 0;


    float animRotAngle = 0.278387f;
    QVector3D animRotAxis = {0.112537f, 0.139021f, 0.983874f};
    QVector3D animTranslate = {0.0514671f, 0.116855f, 0.0471957f};
    QQuaternion qanimRot = QQuaternion::fromAxisAndAngle(animRotAxis, animRotAngle);
    kf.rotation = qanimRot;
    kf.translation = animTranslate;

    Keyframe kfEnd;
    kfEnd.time = 3;

    Keyframe kf2;
    kfEnd.time = 2;

    track.keyframes.append(kf);
    track.keyframes.append(kf2);
    track.keyframes.append(kfEnd);

    animation.tracks.append(track);

    return animation;
}

void Animation::exportAnm(QString filepath) {
    LxStream stream(filepath, LxStream::WriteOnly);
    writeHeader(stream);

    for(Track track : tracks) {
        if(track.bone == "root") {
            Utils::writeString(stream, "Target_CTRL");
        } else {
            Utils::writeString(stream, track.bone);
        }
        int numFrames = fps * length;

        for(int i = 0; i < numFrames; i++) {
            float time = float(i) / float(fps);
            Keyframe kf = track.getKeyframeAt(time);

            QVector3D convertedTrans = skeleton.bone(track.bone).rotation.inverted() * kf.translation;
            QQuaternion rotation = kf.rotation.inverted();

            // Translation
            stream.writeFloat(convertedTrans.x());
            stream.writeFloat(convertedTrans.y());
            stream.writeFloat(convertedTrans.z());

            // Rotation
            stream.writeFloat(rotation.x());
            stream.writeFloat(rotation.y());
            stream.writeFloat(rotation.z());
            stream.writeFloat(rotation.scalar());

            // Scale
            stream.writeFloat(1);
            stream.writeFloat(1);
            stream.writeFloat(1);

            // Another Rotation >.>
            stream.writeFloat(0);
            stream.writeFloat(0);
            stream.writeFloat(0);
            stream.writeFloat(1);
        }
    }

    {
        Utils::writeString(stream, "Bip01");
        int numFrames = fps * length;

        for(int i = 0; i < numFrames; i++) {
            // Translation
            stream.writeFloat(0);
            stream.writeFloat(0);
            stream.writeFloat(0);

            // Rotation
            stream.writeFloat(0);
            stream.writeFloat(0);
            stream.writeFloat(0);
            stream.writeFloat(1);

            // Scale
            stream.writeFloat(0);
            stream.writeFloat(0);
            stream.writeFloat(0);

            // Another Rotation >.>
            stream.writeFloat(0);
            stream.writeFloat(0);
            stream.writeFloat(0);
            stream.writeFloat(1);
        }
    }

    stream.writeQString(extraData.toString());
    stream.close();
}

void Animation::applyBindPose(Skeleton sk) {
    QMap<QString, Track*> bone2track;

    for(Track& track : tracks) {
        bone2track[track.bone] = &track;
    }

    for(auto it = sk.bones.constBegin(); it != sk.bones.constEnd(); ++it) {
        const QString boneName = it.key();

        Bone animationBone = skeleton.bone(boneName);
        Bone bindPoseBone = it.value();

        QQuaternion qRotationChange = bindPoseBone.rotation.inverted() * animationBone.rotation;
        QVector3D translationChange = bindPoseBone.rotation.inverted() * (animationBone.position - bindPoseBone.position);


        if(bone2track.contains(boneName)) {
            Track *track = bone2track[boneName];
            for(Keyframe& keyframe : track->keyframes) {
                keyframe.translation = animationBone.rotation * (bindPoseBone.rotation.inverted() * keyframe.translation + translationChange);
                keyframe.rotation = qRotationChange * keyframe.rotation;
            }
        } else {
            // Track doesn't even exist, so we have to create some pseudo track
            Keyframe kf1, kf2;
            kf1.time = 0;
            kf1.rotation = qRotationChange;
            kf1.translation = kf1.rotation * translationChange;

            kf2 = kf1;
            kf2.time = length;


            Track track;
            track.bone = boneName;

            track.keyframes.append(kf1);
            track.keyframes.append(kf2);

            tracks.append(track);
        }
    }
}

void Animation::setExtraData(ExtraData d) {
    extraData = d;
}

void Animation::writeHeader(LxStream& stream) {
    stream.writeInt(0x24D4E41); // magic number
    stream.writeInt(tracks.length()+1);
    stream.writeInt(fps * length);
    stream.writeInt(fps);
}

Keyframe Track::getKeyframeAt(float time) {
    // TODO: We don't actually add the last keyframe here.
    Keyframe kf1, kf2;

    int i = 0;

    for(i = 0; i < keyframes.length(); i++) {
        Keyframe kf = keyframes[i];

        if(qFuzzyCompare(kf.time, time)) {
            return kf;
        }

        // Loop until we got the keyframe right after the time that we search for, then also take the previous one.
        if(kf.time > time) {
            kf2 = kf;
            kf1 = keyframes[(i-1)%keyframes.length()];
            break;
        }
    }

    if(i == keyframes.length()) {
        qDebug() << "The trailing keyframe is required, but missing :(";
    }

    Keyframe ret;

    float dif = kf2.time - kf1.time;
    float dtime = time - kf1.time;
    float lerpVal = dtime / dif;

    ret.rotation = QQuaternion::slerp(kf1.rotation, kf2.rotation, lerpVal);
    ret.translation = kf1.translation + lerpVal*(kf2.translation - kf1.translation);

//    qDebug() << time << ":" << ret.translation;

    return ret;
}

QString ExtraData::toString() const {
    QString str;
    for(CallbackPoint p : callbackPoints) {
        str.append("CallbackPoint\n{\n");
        str.append("\tname = \"" + p.name + "\"\n");
        str.append(QString("\tframe = %1\n").arg(p.frame));
        str.append("}\n");
    }
    for(CreateEntity p : createEntities) {
        str.append("CreateEntity\n{\n");
        str.append(QString("\tframe = %1\n").arg(p.frame));
        str.append("\tentity = \"" + p.entity + "\"\n");
        str.append("\tattach = \"" + p.attach + "\"\n");
        str.append("}\n");
    }

    return str;
}
