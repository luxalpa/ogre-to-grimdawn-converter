#ifndef UTILS_H
#define UTILS_H

class QDomDocument;
class QString;
class LxStream;

namespace Utils {
    QDomDocument readXMLFile(QString filename);
    void writeString(LxStream &stream, QString str);
}

#endif // UTILS_H
