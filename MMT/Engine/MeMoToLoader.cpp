#include <QJsonParseError>

#include "MeMoToLoader.h"
#include "ErrorDisplayer.h"

QJsonObject MeMoToLoader::loadFromFile(QFile& p_File)
{
    if(!p_File.exists())
    {
        ErrorDisplayer::displayError("File not found!", p_File.fileName());
        return QJsonObject();
    }
    p_File.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray l_FileContent = p_File.readAll();
    p_File.close();

    return MeMoToLoader::loadFromArray(l_FileContent);
}
QJsonObject MeMoToLoader::loadFromArray(const QByteArray& p_Array)
{
    QJsonDocument l_JsonDoc;
    QJsonParseError l_Error;
    l_JsonDoc = QJsonDocument::fromJson(p_Array, &l_Error);

    QJsonObject l_JsonObject;
    if( QJsonParseError::NoError != l_Error.error )
    {
        QByteArray l_CutArray = p_Array;
        l_CutArray.truncate(l_Error.offset);
        unsigned int l_nbLines = l_CutArray.count('\n');
        ErrorDisplayer::displayError("JSON ERROR",
                                     l_Error.errorString() + " at line " + QString::number(l_nbLines));
    }
    else
    {
        l_JsonObject = l_JsonDoc.object();
    }

    return l_JsonObject;
}

void MeMoToLoader::saveToFile(QFile& p_File, const QJsonObject& p_Object)
{
    QByteArray l_Array;
    MeMoToLoader::JsonToArray(p_Object, l_Array);

    p_File.open(QIODevice::WriteOnly | QIODevice::Text);
    p_File.write(l_Array);
    p_File.close();
}
void MeMoToLoader::JsonToArray(const QJsonObject& p_Json, QByteArray& p_rArray)
{
    QJsonDocument l_JSonDoc;
    l_JSonDoc.setObject(p_Json);
    p_rArray = l_JSonDoc.toJson(QJsonDocument::Indented);

    // To avoid portability issues
    p_rArray.replace('\n', QByteArray("\r\n"));
}
