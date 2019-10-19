#include "Utils.h"

#include <QDomDocument>
#include <QString>
#include <QFile>
#include <QDebug>
#include "LxStream.h"

QDomDocument Utils::readXMLFile(QString filename) {
    QDomDocument doc("mydocument");
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Couldn't open file.";
        return doc;
    }
    if (!doc.setContent(&file)) {
        file.close();
        return doc;
    }
    file.close();
    return doc;
}

void Utils::writeString(LxStream &stream, QString str) {
    stream.writeInt(str.length());
    stream.writeQString(str);
}
