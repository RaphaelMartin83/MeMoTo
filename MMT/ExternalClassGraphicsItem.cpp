#include <QProcessEnvironment>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

#include "ClassDiagramScene.h"
#include "ExternalClassGraphicsItem.h"
#include "ConfigWidget.h"
#include "ExternalClassConfiguration.h"

static const char* EXTERNAL_CLASS_CONNECTABLE_NAME = "ExternalClass";
const char* ExternalClassGraphicsItem::SERIALIZABLE_NAME = "ExternalClass";
static const QColor DEFAULT_EXTERNAL_CLASS_COLOR = Qt::yellow;

static ExternalClassConfiguration* s_ConfigurationContext;

ExternalClassGraphicsItem::ExternalClassGraphicsItem(
                            const QPoint& p_Pos,
                            unsigned short p_Width,
                            unsigned short p_Height):
    I_ClassGraphicsItem(p_Pos, QString("ExtClass"), p_Width, p_Height)
  , m_Root()
  , m_Path()
{
    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new ExternalClassConfiguration();
    }

    this->setColor(DEFAULT_EXTERNAL_CLASS_COLOR);
}
ExternalClassGraphicsItem::ExternalClassGraphicsItem(
        const QJsonObject& p_Json):
    I_ClassGraphicsItem(p_Json)
  , m_Root(p_Json.find("Root")->toString())
  , m_Path(p_Json.find("Path")->toString())
{
    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new ExternalClassConfiguration();
    }

    ExternalClassGraphicsItem::refreshDisplay();
}

ExternalClassGraphicsItem::~ExternalClassGraphicsItem()
{

}

void ExternalClassGraphicsItem::refreshDisplay()
{
    QFile l_File;
    QString l_FileString = this->getResolvedFullPath();
    l_File.setFileName(l_FileString);
    l_File.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray l_FileContent = l_File.readAll();
    l_File.close();

    QJsonDocument l_JsonDoc;
    QJsonParseError l_Error;
    l_JsonDoc = QJsonDocument::fromJson(l_FileContent, &l_Error);

    QJsonObject l_JsonObject;
    l_JsonObject = l_JsonDoc.object();

    // Finds myself into the json file
    // Breaks encapsulation, todo: improve
    QJsonObject::iterator l_tmpObjectFound = l_JsonObject.find("ClassDiagram");
    if( l_JsonObject.end() != l_tmpObjectFound )
    {
        l_JsonObject = l_tmpObjectFound->toObject();

        QJsonArray l_JsonArray = l_JsonObject.find(ClassGraphicsItem::SERIALIZABLE_NAME)->toArray();

        for(QJsonArray::Iterator l_ClassesIterator = l_JsonArray.begin();
            l_ClassesIterator < l_JsonArray.end(); l_ClassesIterator++)
        {
            if( this->getName() == l_ClassesIterator->toObject().find("Name")->toString() )
            {
                this->fromExternalJson(l_ClassesIterator->toObject());
            }
        }

    }
    I_ClassGraphicsItem::refreshDisplay();
}

QString ExternalClassGraphicsItem::getResolvedFullPath() const
{
    return s_ConfigurationContext->getResolvedPath(this->getRoot(), this->getPath(), this->getName());
}

const QString& ExternalClassGraphicsItem::getRoot() const
{
    return m_Root;
}
const QString& ExternalClassGraphicsItem::getPath() const
{
    return m_Path;
}
void ExternalClassGraphicsItem::setRootAndPath(const QString& p_Root,
                                                const QString& p_Path)
{
    m_Root = p_Root;
    m_Path = p_Path;
}

void ExternalClassGraphicsItem::select()
{
    I_SquarishGraphicsItem::select();

    if( this->isFullySelected() )
    {
        this->openConfiguration();
    }
}
void ExternalClassGraphicsItem::select(QPoint p_Pos)
{
    I_SquarishGraphicsItem::select(p_Pos);

    if( this->isFullySelected() )
    {
        this->openConfiguration();
    }
}

void ExternalClassGraphicsItem::unselect()
{
    I_SquarishGraphicsItem::unselect();

    this->closeConfiguration();
}

QString ExternalClassGraphicsItem::getSelectableType() const
{
    return SERIALIZABLE_NAME;
}

void ExternalClassGraphicsItem::openConfiguration()
{
    s_ConfigurationContext->registerDiagram(this->getDiagramContainer());
    s_ConfigurationContext->setListener(this->getSelectableType(), this->getID());

    // initializes module with parameters
    s_ConfigurationContext->setName(this->getName());
    s_ConfigurationContext->setRoot(this->getRoot());
    s_ConfigurationContext->setPath(this->getPath());
    s_ConfigurationContext->setColor(this->getColorName());

    // Let's rock
    ConfigWidget::open(s_ConfigurationContext);
}
void ExternalClassGraphicsItem::closeConfiguration()
{
    ConfigWidget::close();
}
void ExternalClassGraphicsItem::applyConfiguration()
{
    this->setName(s_ConfigurationContext->getName());
    this->setRootAndPath(s_ConfigurationContext->getRoot(),
                         s_ConfigurationContext->getPath());
    this->setColor(s_ConfigurationContext->getColor());

    this->refreshDisplay();

    this->getDiagramContainer()->changed(this);

    ConfigWidget::close();
}

QString ExternalClassGraphicsItem::getConnectableType() const
{
    return EXTERNAL_CLASS_CONNECTABLE_NAME;
}

QJsonObject ExternalClassGraphicsItem::toJson()
{
    // Call 2nd parent toJson only to bypass serialization of methods and attributes
    QJsonObject l_MyJson = I_SquarishGraphicsItem::toJson();

    l_MyJson.insert("Root", m_Root);
    l_MyJson.insert("Path", m_Path);

    return l_MyJson;
}
void ExternalClassGraphicsItem::fromJson(QJsonObject p_Json)
{

}

QString ExternalClassGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}

void ExternalClassGraphicsItem::fromExternalJson(QJsonObject p_Json)
{
    // Retrieves class capabilities from external json
    QJsonArray l_MethodsJson = p_Json.find("Methods")->toArray();

    QList<QString> l_NewMethods;

    for(QJsonArray::Iterator l_MethodsIterator = l_MethodsJson.begin();
        l_MethodsIterator < l_MethodsJson.end(); l_MethodsIterator++)
    {
        l_NewMethods.append(l_MethodsIterator->toString());
    }

    QJsonArray l_AttributesJson = p_Json.find("Attributes")->toArray();

    QList<QString> l_NewAttributes;

    for(QJsonArray::Iterator l_AttributesIterator = l_AttributesJson.begin();
        l_AttributesIterator < l_AttributesJson.end(); l_AttributesIterator++)
    {
        l_NewAttributes.append(l_AttributesIterator->toString());
    }

    this->setMethodsList(l_NewMethods);
    this->setAttributesList(l_NewAttributes);
}
