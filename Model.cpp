#include "Model.h"
#include <QtXml/QDomDocument>
#include <QFile>
#include <QDebug>
#include <QtMath>
#include "Utils.h"
#include "LxStream.h"

const float SCALE_FACTOR = 1.0;

Model::Model() {

}

Model Model::fromFile(QString filename) {
    Model m;
    QDomDocument doc = Utils::readXMLFile(filename);

    QDomElement mesh = doc.firstChildElement("mesh");

    m.geometry = getGeometry(mesh);
    m.faces = getFaces(mesh);

    return m;
}

Model Model::dummy() {
    Model m;
    m.geometry.UVs = {
        {0, 0},
        {0, 1},
        {1, 0},
    };

    WeightEntry we;
    we.weights.append({0, 1.f});
//    we.weights.append({2, 0.5f});

    m.geometry.vertexWeights = {
        we, we, we
    };

    m.geometry.vertexNormals = {
        {0, 1.f, 0},
        {0, 1.f, 0},
        {0, 1.f, 0},
    };

    m.geometry.vertexPositions = {
        {0, 0, 0},
        {0, 0, 10.f},
        {7.5f, 0, 0},
    };

    m.faces = {
        {0, 0, 0}
    };

    float anskRotAngle = 2.76351f;
    QVector3D anskRotAxis = {-0.500949f, -0.843208f, -0.195062f};
    QVector3D anskTranslate = {-0.642333f, -0.0427256f, 0.611621f};
    QQuaternion qanskRot = QQuaternion::fromAxisAndAngle(anskRotAxis, anskRotAngle);

    m.skeleton = Skeleton::fromFile("TOWER_DRAGON.SKELETON.xml");
    m.addPseudoBone("Bip01", "root");
    m.addPseudoBone("Bip01 Pelvis", "Bip01");
    m.addPseudoBone("Bip01 Spine", "Bip01 Pelvis");
    m.addPseudoBone("Bip01 Spine1", "Bip01 Spine");
    m.addPseudoBone("Bip01 Neck", "Bip01 Spine1");


//    Skeleton otherSkeleton = Skeleton::fromFile("TOWER_DRAGON.SKELETON.xml", false);

//    for(auto it = otherSkeleton.bones.constBegin(); it != otherSkeleton.bones.constEnd(); ++it) {
//        Bone b = it.value();
//        if(b.name == "root") {
//            continue;
//        }
//        if(b.parent == "root") {
//            m.skeleton.bones["root"].children.append(b.name);
//        }
//        b.id += 100;
//        m.skeleton.bones.insert(b.name, b);
//    }

//    m.skeleton.bones.insert("root", {"root", {"Bone1", "Bone2"}, "", QVector3D(), QQuaternion(), 0});
//    m.skeleton.bones.insert("Bone1", {"Bone1", {}, "root", anskTranslate, qanskRot, 1});
//    m.skeleton.bones.insert("Bone2", {"Bone2", {}, "root", QVector3D(-0.590866, 0.0741294, 0.658817), QQuaternion(), 2});

    return m;
}


void Model::addPseudoBone(QString name, QString parent)
{
    skeleton.bones[parent].children.append(name);
    skeleton.bones.insert(name, {name, {}, parent, QVector3D(100, 0, 0), QQuaternion(), 100});
}

void Model::writeHeader(LxStream &stream) {
    stream.setEndianness(LxStream::LittleEndian);
    stream.write(0x074C444D); // Magic Number
    stream.writeInt(1); // NumMaterials
    stream.writeInt(1); // NumNodes
    stream.writeInt(skeleton.numBones()); // NumBones
    stream.writeInt(0);
}

void writeParamFloat3(LxStream &stream, QString name, float v1, float v2, float v3) {
    Utils::writeString(stream, name);
    stream.writeInt(0xC);
    stream.writeFloat(v1);
    stream.writeFloat(v2);
    stream.writeFloat(v3);
}

void writeParamFloat(LxStream &stream, QString name, float v) {
    Utils::writeString(stream, name);
    stream.writeInt(0xA);
    stream.writeFloat(v);
}

void writeParamString(LxStream &stream, QString name, QString value) {
    Utils::writeString(stream, name);
    stream.writeInt(0x7);
    Utils::writeString(stream, value);
}

void Model::writeShaderParams(LxStream &stream) {
    Utils::writeString(stream, "Shaders/standardskinned.ssh");
    stream.writeInt(6);

    writeParamFloat3(stream, "specularColor", 0.0, 0.0, 0.0);
    writeParamFloat(stream, "specularPower", 0.0);
    writeParamFloat(stream, "outlineThickness", 1.0);
    writeParamString(stream, "baseTexture", "Creatures/dragon/dragon.tex");
    writeParamString(stream, "bumpTexture", "");
    writeParamString(stream, "specTexture", "");
}

void Model::writeMesh(LxStream &stream, QMap<int, int> boneConv) {
    stream.writeInt(1);
    long long posTotalSize = stream.pos();
    stream.writeInt(0); // placeholder

    // Data
    Utils::writeString(stream, "Dragon");
    stream.writeInt(0);
    stream.writeInt(0);

    stream.writeInt(faces.length());
    stream.writeInt(geometry.vertexPositions.length());
    stream.writeInt(geometry.vertexNormals.length());
    stream.writeInt(geometry.UVs.length());
    stream.writeInt(geometry.vertexWeights.length()); // NumWeights
    stream.writeInt(0);

    for(Triangle face : faces) {
        stream.writeInt(face.v1); // vertex
        stream.writeInt(face.v1); // normal
        stream.writeInt(face.v1); // UVs
        stream.writeInt(0);
        stream.writeInt(face.v1); // Weights

        stream.writeInt(face.v2); // vertex
        stream.writeInt(face.v2); // normal
        stream.writeInt(face.v2); // UVs
        stream.writeInt(0);
        stream.writeInt(face.v2); // Weights

        stream.writeInt(face.v3); // vertex
        stream.writeInt(face.v3); // normal
        stream.writeInt(face.v3); // UVs
        stream.writeInt(0);
        stream.writeInt(face.v3); // Weights

        stream.writeInt(0);
        stream.writeInt(0);
    }

    for(QVector3D vertex : geometry.vertexPositions) {
        stream.writeFloat(vertex.x() * SCALE_FACTOR);
        stream.writeFloat(vertex.y() * SCALE_FACTOR);
        stream.writeFloat(vertex.z() * SCALE_FACTOR);
    }

    for(QVector3D normal : geometry.vertexNormals) {
        stream.writeFloat(normal.x() * SCALE_FACTOR);
        stream.writeFloat(normal.y() * SCALE_FACTOR);
        stream.writeFloat(normal.z() * SCALE_FACTOR);
    }

    for(Vector2 texCoord : geometry.UVs) {
        stream.writeFloat(texCoord.x * SCALE_FACTOR);
        stream.writeFloat((1-texCoord.y) * SCALE_FACTOR);
    }

    for(WeightEntry we : geometry.vertexWeights) {
        stream.writeInt(we.weights.length());
        for(Weight weight : we.weights) {
            stream.writeInt(boneConv[weight.boneID]);
            stream.writeFloat(weight.weight);
        }
    }

    float endPos = stream.pos();
    stream.seek(posTotalSize);
    stream.writeInt(endPos - posTotalSize - 4);
}

// Returns a map that maps from oldBoneIndex to newBoneIndex
QMap<int, int> Model::writeBones(LxStream &stream) {
    QMap<int, int> boneConv;

    int curPos = 1;
    int curIndex = 0;

    QList<QString> children = {"root"};
    QList<QString> nextChildren;

    while(true) {
        for(QString name : children) {
            Bone node = skeleton.bone(name);

            if(node.name == "root") {
                Utils::writeString(stream, "Target_CTRL");
            } else {
                Utils::writeString(stream, node.name);
            }
            stream.writeInt(curPos);
            stream.writeInt(node.children.size());

            QMatrix3x3 mat = node.rotation.toRotationMatrix();

            stream.writeFloat(mat.constData()[0]);
            stream.writeFloat(mat.constData()[1]);
            stream.writeFloat(mat.constData()[2]);
            stream.writeFloat(mat.constData()[3]);
            stream.writeFloat(mat.constData()[4]);
            stream.writeFloat(mat.constData()[5]);
            stream.writeFloat(mat.constData()[6]);
            stream.writeFloat(mat.constData()[7]);
            stream.writeFloat(mat.constData()[8]);


            stream.writeFloat(node.position.x());
            stream.writeFloat(node.position.y());
            stream.writeFloat(node.position.z());

            curPos += node.children.size();
            nextChildren.append(node.children);

            boneConv[node.id] = curIndex;
            curIndex++;
        }

        if(nextChildren.isEmpty()) {
            break;
        }

        children = nextChildren;
        nextChildren.clear();
    }

    return boneConv;
}

void Model::exportMDL(QString filename) {

    LxStream stream(filename, LxStream::WriteOnly);

    QMap<int, int> boneConv;

    writeHeader(stream);
    writeShaderParams(stream);

    if(skeleton.numBones() != 0) {
        boneConv = writeBones(stream);
    }
    writeMesh(stream, boneConv);

    stream.close();
}

void Model::addSkeleton(Skeleton sk) {
    skeleton = sk;
}



Geometry Model::getGeometry(QDomElement mesh) {
    Geometry geo;
    QDomElement sharedgeometry = mesh.firstChildElement("sharedgeometry");
    QDomNodeList vertexBuffers = sharedgeometry.childNodes();
    for(int i = 0; i < vertexBuffers.length(); i++) {
        QDomElement n = vertexBuffers.item(i).toElement();
        if(n.nodeName() != "vertexbuffer") {
            continue;
        }

        bool hasPositions = n.attribute("positions") == "true";
        bool hasNormals = n.attribute("normals") == "true";
        int textureCoords = n.attribute("texture_coords", "0").toInt();
        int textureCoordDimensions = n.attribute("texture_coord_dimensions_0", "0").toInt();

        QDomNodeList vertices = n.childNodes();
        for(int j = 0; j < vertices.length(); j++) {
            QDomElement vertex = vertices.item(j).toElement();
            if(hasPositions) {
                QDomElement pos = vertex.firstChildElement("position");
                float x = pos.attribute("x").toFloat();
                float y = pos.attribute("y").toFloat();
                float z = pos.attribute("z").toFloat();
                geo.vertexPositions.append({x, y, z});
            }

            if(hasNormals) {
                QDomElement pos = vertex.firstChildElement("normal");
                float x = pos.attribute("x").toFloat();
                float y = pos.attribute("y").toFloat();
                float z = pos.attribute("z").toFloat();
                geo.vertexNormals.append({x, y, z});
            }

            if(textureCoords == 1 && textureCoordDimensions == 2) {
                QDomElement pos = vertex.firstChildElement("texcoord");
                double u = pos.attribute("u").toDouble();
                double v = pos.attribute("v").toDouble();
                geo.UVs.append({u, v});
            }
        }
    }

    geo.vertexWeights = getWeights(mesh);

    return geo;
}

QList<Triangle> Model::getFaces(QDomElement mesh) {
    QList<Triangle> allFaces;

    QDomNodeList submeshes = mesh.firstChildElement("submeshes").childNodes();
    for(int i = 0; i < submeshes.length(); i++) {
        QDomElement submesh = submeshes.item(i).toElement();
        QDomNodeList faces = submesh.firstChildElement("faces").childNodes();
        for(int j = 0; j < faces.length(); j++) {
            QDomElement face = faces.item(j).toElement();
            int v1 = face.attribute("v1").toInt();
            int v2 = face.attribute("v2").toInt();
            int v3 = face.attribute("v3").toInt();

            allFaces.append({v1, v2, v3});
        }
    }

    return allFaces;
}

QList<WeightEntry> Model::getWeights(QDomElement mesh) {
    QList<WeightEntry> weights;

    QDomNodeList boneAssignments = mesh.firstChildElement("boneassignments").childNodes();

    for(int i = 0; i < boneAssignments.length(); i++) {
        QDomElement assignment = boneAssignments.item(i).toElement();
        int vertex = assignment.attribute("vertexindex").toInt();
        int bone = assignment.attribute("boneindex").toInt();
        float weight = assignment.attribute("weight").toFloat();

        while(weights.length() <= vertex) {
            weights.append(WeightEntry());
        }

        WeightEntry &we = weights[vertex];
        we.weights.append({bone, weight});
    }

    return weights;
}
