#ifndef LXSTREAM_H
#define LXSTREAM_H

// VERSION: 1.2.1 beta

#if !defined(USE_QT) && defined(QT_VERSION)
#define USE_QT
#endif

#ifdef USE_QT
#include <QString>
#include <QFile>
#include <QBuffer>
#include <QDebug>
#include <QFile>
#endif
#include <stack>
#include <stdio.h>

#ifdef _MSC_VER
    #define BYTESWAP_16 _byteswap_ushort
    #define BYTESWAP_32 _byteswap_ulong
    #define BYTESWAP_64 _byteswap_uint64
#else
    #define BYTESWAP_16 __builtin_bswap16
    #define BYTESWAP_32 __builtin_bswap32
    #define BYTESWAP_64 __builtin_bswap64
#endif

class LxStreamDevice
{
public:
    virtual ~LxStreamDevice() = 0;
    virtual void close() = 0;
    virtual char *getData(int length) = 0;
    virtual bool isOpen() const = 0;
    virtual void seek(long long newPos) = 0;
    virtual long long pos() const = 0;
    virtual void writeData(const char *data, int length) = 0;
    virtual bool atEnd() const = 0;
    virtual long long size() const = 0;
};

class LxStream
{
public:
    enum OpenMode {
#ifdef USE_QT
        NotOpen = QIODevice::NotOpen,
        ReadOnly = QIODevice::ReadOnly,
        WriteOnly = QIODevice::WriteOnly,
        ReadWrite = QIODevice::ReadWrite,
        Append = QIODevice::Append,
        Truncate = QIODevice::Truncate,
        Text = QIODevice::Text,
        Unbuffered = QIODevice::Unbuffered
#else
        NotOpen = 0,
        ReadOnly = 1,
        WriteOnly = 2,
        ReadWrite = ReadOnly | WriteOnly,
        Append = 4,
        Truncate = 8,
        Text = 0x10,
        Unbuffered = 0x20
#endif
    };

    enum Endianness {
        LittleEndian = 0,
        BigEndian = 1
    };

    LxStream();

    ~LxStream();

    void openData(char *data, int length);
    void openFile(const char *filename, OpenMode mode);

    void close();
    bool isOpen() const;
    void seek(long long newPos);
    void skip(int num);
    void rewind(int amount);
    void push();
    void pop();
    void clearStack();
    long long pos() const;
	long long size() const;
    bool atEnd() const;
    void setEndianness(Endianness e);


    template <typename T> T read() {
        char* c = device->getData(sizeof(T));
        T v = *(T*)c;
        delete c;
        return v;
    }


    template <typename T> void write(const T d) {
        device->writeData((const char*)&d, sizeof(T));
    }

    char        readChar();
    short       readShort();
    int         readInt();
    float       readFloat();
    long long   readInt64();
    char *      readZString(int buffersize = 1024);
    char *      readData(int length);

    void writeChar(const char c);
    void writeShort(const short s);
    void writeInt(const int i);
    void writeFloat(const float f);
    void writeInt64(const long long i);
    void writeZString(const char *str);
    void writeData(const char *data, int length);

    // Qt Functions
#ifdef USE_QT
    LxStream(QString filename, OpenMode mode);
    LxStream(QByteArray *arr);

    void openQFile(QString filename, OpenMode mode);
    void openByteArray(QByteArray *ba);
    QByteArray readAll();
    QByteArray readByteArray(int length);
    void writeByteArray(QByteArray data);
    QString readQString();
    QString readLine();
    void writeQString(const QString s);
#else
    LxStream(const char * filename, OpenMode mode);
#endif


private:
    void init();

    std::stack<long long> positions;
    LxStreamDevice *device;
    Endianness endianness;

    // disable copies:
    LxStream(const LxStream &);
    LxStream &operator=(const LxStream &);
};

class LxFileStream : public LxStreamDevice
{
public:
    LxFileStream(const char *filename, LxStream::OpenMode mode);
    virtual ~LxFileStream();
    virtual void close();
    virtual bool isOpen() const { return m_isOpen; }
    virtual char *getData(int length);
    virtual void seek(long long newPos);
    virtual long long pos() const;
    virtual void writeData(const char *data, int length);
    virtual bool atEnd() const;
    virtual long long size() const;
private:
    bool getOpenMode(LxStream::OpenMode mode, char* newMode);

    bool m_isOpen;
    FILE *file;
};

class LxCharArrayStream : public LxStreamDevice
{
public:
    LxCharArrayStream(char *d, int length);
    virtual ~LxCharArrayStream() {}
    virtual void close() {}
    virtual bool isOpen() const { return true; }
    virtual char *getData(int length);
    virtual void seek(long long newPos);
    virtual long long pos() const { return position; }
    virtual void writeData(const char *data, int length);
    virtual bool atEnd() const { return position == m_size; }
    virtual long long size() const { return m_size; }
private:
    int m_size;
    int position;
    char *data;
};

#ifdef USE_QT

class LxQFileStream : public LxStreamDevice
{
public:
    LxQFileStream(QString filename, LxStream::OpenMode mode);
    virtual ~LxQFileStream();
    virtual void close();
    virtual bool isOpen() const;
    virtual char *getData(int length);
    virtual void seek(long long newPos);
    virtual long long pos() const;
    virtual void writeData(const char *data, int length);
    virtual bool atEnd() const;
    virtual long long size() const;

private:    
    QFile file;
};


class LxQByteArrayStream : public LxStreamDevice
{
public:
    LxQByteArrayStream(QByteArray *d);
    virtual ~LxQByteArrayStream();
    virtual void close() {  buf.close(); }
    virtual bool isOpen() const { return buf.isOpen(); }
    virtual char *getData(int length);
    virtual void seek(long long newPos);
    virtual long long pos() const { return buf.pos(); }
    virtual void writeData(const char *data, int length);
    virtual bool atEnd() const { return buf.atEnd(); }
    virtual long long size() const { return buf.size(); }
private:
    QBuffer buf;
};

#endif // USE_QT









namespace {

inline void error(const char *errorText) {
#ifdef USE_QT
    qDebug() << errorText;
#else
    printf(errorText);
#endif
}

}


inline LxStreamDevice::~LxStreamDevice()
{
}

inline LxStream::LxStream()
{
    init();
}

#ifndef USE_QT

inline LxStream::LxStream(const char * filename, LxStream::OpenMode mode)
{
    init();
    openFile(filename, mode);
}

#endif

inline void LxStream::init()
{
    device = 0;
    setEndianness(LittleEndian);
}

inline LxStream::~LxStream()
{
    delete device;
}

inline void LxStream::openData(char *data, int length)
{
    if(device != 0)
        delete device;
    device = new LxCharArrayStream(data, length);
}

inline void LxStream::openFile(const char *filename, LxStream::OpenMode mode)
{
    if(device != 0)
        delete device;
    device = new LxFileStream(filename, mode);
}

inline void LxStream::close()
{
    device->close();
}

inline bool LxStream::isOpen() const
{
    return device->isOpen();
}

inline void LxStream::seek(long long newPos)
{
    device->seek(newPos);
}

inline void LxStream::skip(int num)
{
    device->seek(pos() + num);
}

inline void LxStream::rewind(int amount)
{
    device->seek(pos() - amount);
}

inline void LxStream::push()
{
    positions.push(device->pos());
}

inline void LxStream::pop()
{
    long long position = positions.top();
    positions.pop();
    seek(position);
}

inline void LxStream::clearStack()
{
    positions = std::stack<long long>();
}

inline long long LxStream::pos() const
{
    return device->pos();
}

inline long long LxStream::size() const
{
	return device->size();
}

inline bool LxStream::atEnd() const
{
    return device->atEnd();
}

inline void LxStream::setEndianness(LxStream::Endianness e)
{
    endianness = e;
}

inline char LxStream::readChar()
{
    return read<char>();
}

inline short LxStream::readShort()
{
    if(endianness == BigEndian)
        return BYTESWAP_16(read<short>());
    else
        return read<short>();
}

inline int LxStream::readInt()
{
    if(endianness == BigEndian)
        return BYTESWAP_32(read<int>());
    else
        return read<int>();
}

inline float LxStream::readFloat()
{
    if(endianness == BigEndian) {
        float f = read<float>();
        int bf = BYTESWAP_32((int&)f);
        return (float&)bf;
    } else {
        return read<float>();
    }
}

inline long long LxStream::readInt64()
{
    if(endianness == BigEndian)
        return BYTESWAP_64(read<long long>());
    else
        return read<long long>();
}

inline char *LxStream::readZString(int bufferSize)
{
    char *str = new char[bufferSize];
    char c = -1;
    int p = 0;
    while(!atEnd() && p < bufferSize) {
        c = read<char>();
        str[p] = c;
        p++;
        if(c == 0) {
            char *newStr = new char[p];
            memcpy(newStr, str, p);
            delete str;
            return newStr;
        }
    }
    if(p == bufferSize)
        return str;
    else
        return 0;
}

inline char *LxStream::readData(int length)
{
    return device->getData(length);
}

inline void LxStream::writeZString(const char *str)
{
    int x = 0;
    while(str[x] != 0) {
        write<char>(str[x]);
        x++;
    }
    write<char>(0);
}

inline void LxStream::writeData(const char *data, int length)
{
    device->writeData(data, length);
}

inline void LxStream::writeChar(const char c)
{
    write<char>(c);
}

inline void LxStream::writeShort(const short s)
{
    write<short>(s);
}

inline void LxStream::writeInt(const int i)
{
    write<int>(i);
}

inline void LxStream::writeFloat(const float f)
{
    write<float>(f);
}

inline void LxStream::writeInt64(const long long i)
{
    write<long long>(i);
}

template<> inline char* LxStream::read<char*>() {
    return readZString(1000);
}

template<> inline void LxStream::write<const char*>(const char *str) {
    writeZString(str);
}

#ifdef USE_QT
template<> inline QString LxStream::read<QString>() {
    return readQString();
}
template<> inline void LxStream::write<QString>(QString str) {
    writeQString(str);
}
#endif

inline LxCharArrayStream::LxCharArrayStream(char *d, int length)
{
    this->m_size = length;
    this->position = 0;
    this->data = d;
}

inline char *LxCharArrayStream::getData(int length)
{
    char *d = new char[length];
    if(position + length > m_size) {
        int n = m_size - position;
        memcpy(d, data+position, n);
        memset(d+n, 0, length - m_size);
        position = m_size;
        error("Error: Reading above end of stream!!");
    } else {
        memcpy(d, data+position, length);
        position += length;
    }
    return d;
}

inline void LxCharArrayStream::seek(long long newPos)
{
    if(newPos >= 0 && newPos <= m_size)
        position = newPos;
    else if(newPos > m_size) {
        error("Error: Seeking above end of stream!!");
        position = m_size;
    } else if(newPos < 0) {
        error("Error: Seeking below beginning of stream!!");
        position = 0;
    }
}

inline void LxCharArrayStream::writeData(const char *d, int length)
{
    if(position + length > m_size) {
        int n = m_size - position;
        memcpy(data+position, d, n);
        position = m_size;
        error("Error: Writing above end of stream!!");
    } else {
        memcpy(data+position, d, length);
        position += length;
    }
}

// TODO: This needs to work with files > 2 GiB

inline LxFileStream::LxFileStream(const char *filename, LxStream::OpenMode mode)
{
    m_isOpen = false;
    char omode[4];
    if(!getOpenMode(mode, omode))
        return;

    file = fopen(filename, omode);
    m_isOpen = file != NULL;
    if(!m_isOpen)
        return;
}

inline LxFileStream::~LxFileStream()
{
    if(isOpen())
        close();
}

inline void LxFileStream::close()
{
    fclose(file);
}

inline char *LxFileStream::getData(int length)
{
    char * buf = new char[length];
    fread(buf, length, 1, file);
    return buf;
}

inline void LxFileStream::seek(long long newPos)
{
    fseek(file, newPos, SEEK_SET);
}

inline long long LxFileStream::pos() const
{
    return ftell(file);
}

inline void LxFileStream::writeData(const char *data, int length)
{
    fwrite(data, length, 1, file);
}

inline bool LxFileStream::atEnd() const
{
    return size() == pos();
}

inline long long LxFileStream::size() const
{
    long long p = pos();
    fseek(file, 0, SEEK_END);
    long long v = ftell(file);
    fseek(file, p, SEEK_SET);

    return v;
}

inline bool LxFileStream::getOpenMode(LxStream::OpenMode mode, char *newMode)
{
    memset(newMode, 0, 4);
    if(mode == LxStream::ReadOnly) {
        newMode[0] = 'r';
        newMode[1] = 'b';
        return true;
    }
    if(mode == LxStream::ReadWrite) {
        newMode[0] = 'r';
        newMode[1] = '+';
        newMode[2] = 'b';
        return true;
    }
    if(mode == LxStream::WriteOnly) {
        newMode[0] = 'w';
        newMode[1] = 'b';
        return true;
    }
    error("LxFileStream: Unsupported Mode!");
    return false;
}

#ifdef USE_QT

inline void LxStream::openQFile(QString filename, LxStream::OpenMode mode)
{
    if(device != 0)
        delete device;
    device = new LxQFileStream(filename, mode);
}


inline LxStream::LxStream(QString filename, LxStream::OpenMode mode)
{
    init();
    openQFile(filename, mode);
}

inline LxStream::LxStream(QByteArray *arr)
{
    init();
    openByteArray(arr);
}

inline void LxStream::openByteArray(QByteArray *ba)
{
    if(device != 0)
        delete device;
    device = new LxQByteArrayStream(ba);
}

inline QByteArray LxStream::readAll()
{
    return readByteArray(size() - pos());
}

inline QByteArray LxStream::readByteArray(int length)
{
    char * d = device->getData(length);
    QByteArray ret(d, length);
    delete d;
    return ret;
}

inline void LxStream::writeByteArray(QByteArray data)
{
    device->writeData(data.constData(), data.size());
}

inline QString LxStream::readQString()
{
    QString str;
    char c = -1;
    while(!atEnd()) {
        c = read<char>();
        if(c == 0)
            return str;
        str.append(c);
    }
    return str;
}

inline void LxStream::writeQString(const QString s)
{
    writeByteArray(s.toLatin1());
}

inline QString LxStream::readLine()
{
    QString str;
    char c = -1;
    do {
        c = read<char>();
        str.append(c);
    } while(c != 0 && c != '\n' && !atEnd());
    return str;
}

inline LxQFileStream::LxQFileStream(QString filename, LxStream::OpenMode mode)
{
    file.setFileName(filename);
    file.open((QIODevice::OpenMode) mode);
}

inline LxQFileStream::~LxQFileStream()
{
    if(isOpen())
        close();
}

inline void LxQFileStream::close()
{
    file.close();
}

inline bool LxQFileStream::isOpen() const
{
    return file.isOpen();
}

inline char *LxQFileStream::getData(int length)
{
    char *d = new char[length];
    file.read(d, length);
    return d;
}

inline void LxQFileStream::seek(long long newPos)
{
    file.seek(newPos);
}

inline long long LxQFileStream::pos() const
{
    return file.pos();
}

inline void LxQFileStream::writeData(const char *data, int length)
{
    file.write(data, length);
}

inline bool LxQFileStream::atEnd() const
{
    return file.atEnd();
}

inline long long LxQFileStream::size() const
{
    return file.size();
}

inline LxQByteArrayStream::LxQByteArrayStream(QByteArray *d)
{
    buf.setBuffer(d);
    buf.open(QIODevice::ReadWrite);
}

inline LxQByteArrayStream::~LxQByteArrayStream()
{
    if(isOpen()) {
        close();
    }
}

inline char *LxQByteArrayStream::getData(int length)
{
    char *d = new char[length];
    buf.read(d, length);
    return d;
}

inline void LxQByteArrayStream::seek(long long newPos)
{
    buf.seek(newPos);
}

inline void LxQByteArrayStream::writeData(const char *data, int length)
{
    buf.write(data, length);
}

#endif








#endif // LXSTREAM_H
