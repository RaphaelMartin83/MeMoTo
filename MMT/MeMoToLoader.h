#ifndef MEMOTOLOADER_H
#define MEMOTOLOADER_H

#include <QFile>
#include <QJsonObject>
#include <QByteArray>

class MeMoToLoader
{
public:
    static QJsonObject loadFromFile(QFile& p_File);
    static QJsonObject loadFromArray(const QByteArray& p_Array);

    static void saveToFile(QFile& p_File, const QJsonObject& p_Object);
    static void JsonToArray(const QJsonObject& p_Json, QByteArray& p_rArray);
};

#endif // MEMOTOLOADER_H
