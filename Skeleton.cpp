#include "Skeleton.h"
#include "Utils.h"
#include <QDomElement>
#include <QDomDocument>
#include <QtMath>

Skeleton::Skeleton()
{

}

int Skeleton::numBones() const
{
    return bones.size();
}

Bone Skeleton::bone(QString name) const
{
    return bones[name];
}

QMap<QString, Bone> Skeleton::getBoneInfo(QDomElement skeleton) {
    QMap<QString, Bone> bones;

    QDomNodeList boneList = skeleton.firstChildElement("bones").childNodes();
    for(int i = 0; i < boneList.length(); i++) {
        QDomElement bone = boneList.item(i).toElement();
        int id = bone.attribute("id").toInt();
        if(i != id) {
            qDebug() << "Error: IDs are not in order :(";
        }
        QString name = bone.attribute("name");

        // Make sure equipped items don't recognize these bones.
        if(name != "root") {
            name = "Dx_" + name;
        }

        QDomElement pos = bone.firstChildElement("position");
        QVector3D position = {
            pos.attribute("x").toFloat(),
            pos.attribute("y").toFloat(),
            pos.attribute("z").toFloat()
        };

        QDomElement rot = bone.firstChildElement("rotation");
        float angle = rot.attribute("angle").toFloat();

        QDomElement axisE = rot.firstChildElement("axis");
        QVector3D axis = {
            axisE.attribute("x").toFloat(),
            axisE.attribute("y").toFloat(),
            axisE.attribute("z").toFloat(),
        };

        Bone b;
        b.name = name;

        b.position = position;
        b.rotation = QQuaternion::fromAxisAndAngle(axis, qRadiansToDegrees(angle));
        b.id = bone.attribute("id").toInt();

        bones.insert(name, b);
    }

    return bones;
}

Skeleton Skeleton::fromFile(QString filename, bool withPrefix)
{
    QDomDocument doc = Utils::readXMLFile(filename);
    return fromDocument(doc, withPrefix);
}

Skeleton Skeleton::fromDocument(QDomDocument doc, bool withPrefix)
{
    QDomElement sk = doc.firstChildElement("skeleton");
    QMap<QString, Bone> skeleton = getBoneInfo(sk);

    QDomElement boneHierarchy = sk.firstChildElement("bonehierarchy");
    QDomNodeList boneparents = boneHierarchy.childNodes();
    for(int i = 0; i < boneparents.length(); i++) {
        QDomElement boneparent = boneparents.item(i).toElement();
        QString name = boneparent.attribute("bone");

        if(withPrefix && name != "root") {
            name = "Dx_" + name;
        }

        QString parent = boneparent.attribute("parent");

        if(withPrefix && parent != "root") {
            parent = "Dx_" + parent;
        }

        skeleton[name].parent = parent;
        skeleton[parent].children.append(name);
    }

    Skeleton ret;
    ret.bones = skeleton;

    return ret;
}
