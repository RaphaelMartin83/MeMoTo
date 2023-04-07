#include <QJsonArray>

#include "I_ClassGraphicsItem.h"
#include "MeMoToThemeDefinition.h"

static const unsigned short CLASS_TEXT_LABELS_HEIGHT = 15U;

I_ClassGraphicsItem::I_ClassGraphicsItem(const QPoint& p_Pos, const QString& p_Name,
                            unsigned short p_Width,
                            unsigned short p_Height):
    I_SquarishGraphicsItem(p_Pos, p_Name, p_Width, p_Height)
  , m_Methods()
  , m_Attributes()
  , m_isContentToHide(false)
{
    m_ClassContentToDisplay = new QGraphicsTextItem();
    this->addToGroup(m_ClassContentToDisplay);

    I_ClassGraphicsItem::refreshDisplay();
}

I_ClassGraphicsItem::I_ClassGraphicsItem(const QJsonObject& p_Json):
    I_SquarishGraphicsItem(p_Json)
  , m_Methods()
  , m_Attributes()
  , m_isContentToHide(false)
{
    QJsonObject::const_iterator l_FoundContentToHide = p_Json.find("HideContent");
    if( p_Json.end() != l_FoundContentToHide )
    {
        this->setContentToHide(l_FoundContentToHide->toString().toLower() == "true");
    }
    else
    {
        this->setContentToHide(false);
    }

    QJsonObject::const_iterator l_FoundMethods = p_Json.find("Methods");
    if( p_Json.end() != l_FoundMethods )
    {
        QJsonArray l_MethodsJson = l_FoundMethods->toArray();
        for( unsigned short i_methods = 0U; i_methods < l_MethodsJson.count(); i_methods++ )
        {
            m_Methods.append(l_MethodsJson[i_methods].toString());
        }
    }

    QJsonObject::const_iterator l_FoundAttributes = p_Json.find("Attributes");
    if( p_Json.end() != l_FoundAttributes )
    {
        QJsonArray l_AttributesJson = l_FoundAttributes->toArray();
        for( unsigned short i_attributes = 0U; i_attributes < l_AttributesJson.count(); i_attributes++ )
        {
            m_Attributes.append(l_AttributesJson[i_attributes].toString());
        }
    }

    m_ClassContentToDisplay = new QGraphicsTextItem();
    this->addToGroup(m_ClassContentToDisplay);

    I_ClassGraphicsItem::refreshDisplay();
}

I_ClassGraphicsItem::~I_ClassGraphicsItem()
{

}

void I_ClassGraphicsItem::refreshDisplay()
{
    I_SquarishGraphicsItem::refreshDisplay();

    this->refreshText();
    m_ClassContentToDisplay->setPos(this->getPos().x() - this->getWidth()/2 + 10U,
                                      this->getPos().y() - this->getHeight()/2 + CLASS_TEXT_LABELS_HEIGHT*2);
    m_ClassContentToDisplay->setDefaultTextColor(TEXT_GRAPHICS_COLOR);
    this->setZValue(getDepthFromPlan(this->getPlan()));
}

void I_ClassGraphicsItem::setMethodsList(const QList<QString>& p_Methods)
{
    m_Methods = p_Methods;

    this->refreshText();
}
const QList<QString>& I_ClassGraphicsItem::getMethodsList()
{
    return m_Methods;
}
void I_ClassGraphicsItem::setAttributesList(const QList<QString>& p_Attributes)
{
    m_Attributes = p_Attributes;

    this->refreshText();
}
const QList<QString>& I_ClassGraphicsItem::getAttributesList()
{
    return m_Attributes;
}

bool I_ClassGraphicsItem::isContentToHide() const
{
    return m_isContentToHide;
}
void I_ClassGraphicsItem::setContentToHide(bool p_isContentToHide)
{
    m_isContentToHide = p_isContentToHide;
}

QStringList I_ClassGraphicsItem::getSearchFields() const
{
    QStringList l_Ret;

    l_Ret.append("Name");
    l_Ret.append("Attributes");
    l_Ret.append("Methods");

    return l_Ret;
}
QString I_ClassGraphicsItem::getDataFromField(const QString& p_FieldName) const
{
    QString l_Ret;

    if ( "Name" == p_FieldName )
    {
        l_Ret = this->getName();
    }
    else if( "Attributes" == p_FieldName )
    {
        for( unsigned short i_attributes = 0U; i_attributes < m_Attributes.count(); i_attributes++ )
        {
            l_Ret += m_Attributes[i_attributes] + ';';
        }
    }
    else if( "Methods" == p_FieldName )
    {
        for( unsigned short i_methods = 0U; i_methods < m_Methods.count(); i_methods++ )
        {
            l_Ret += m_Methods[i_methods] + ';';
        }
    }

    return l_Ret;
}

QJsonObject I_ClassGraphicsItem::toJson() const
{
    QJsonArray l_MethodsJson;

    for( unsigned short i_methods = 0U; i_methods < m_Methods.count(); i_methods++ )
    {
        l_MethodsJson.append(m_Methods[i_methods]);
    }

    QJsonArray l_AttributesJson;
    for( unsigned short i_attributes = 0U; i_attributes < m_Attributes.count(); i_attributes++ )
    {
        l_AttributesJson.append(m_Attributes[i_attributes]);
    }

    QJsonObject l_MyJson;
    l_MyJson = I_SquarishGraphicsItem::toJson();

    l_MyJson.insert("Methods", l_MethodsJson);
    l_MyJson.insert("Attributes", l_AttributesJson);
    l_MyJson.insert("HideContent", this->isContentToHide()?"true":"false");

    return l_MyJson;
}

void I_ClassGraphicsItem::fromJson(const QJsonObject& p_Json)
{
    I_SquarishGraphicsItem::fromJson(p_Json);

    QJsonObject::const_iterator l_FoundContentToHide = p_Json.find("HideContent");
    if( p_Json.end() != l_FoundContentToHide )
    {
        this->setContentToHide(QString("true") == l_FoundContentToHide->toString().toLower());
    }
    else
    {
        this->setContentToHide(false);
    }

    QJsonObject::const_iterator l_FoundMethods = p_Json.find("Methods");
    m_Methods.clear();
    if( p_Json.end() != l_FoundMethods )
    {
        QJsonArray l_MethodsJson = l_FoundMethods->toArray();
        for(QJsonArray::iterator l_methods = l_MethodsJson.begin();
            l_methods < l_MethodsJson.end(); l_methods++ )
        {
            m_Methods.append(l_methods->toString());
        }
    }
    QJsonObject::const_iterator l_FoundAttributes = p_Json.find("Attributes");
    m_Attributes.clear();
    if( p_Json.end() != l_FoundAttributes )
    {
        QJsonArray l_AttributesJson = l_FoundAttributes->toArray();
        for(QJsonArray::iterator l_attributes = l_AttributesJson.begin();
            l_attributes < l_AttributesJson.end(); l_attributes++)
        {
            m_Attributes.append(l_attributes->toString());
        }
    }
    this->refreshDisplay();
}

void I_ClassGraphicsItem::refreshText()
{
    // If to hide then nothing to display
    if( this->isContentToHide() )
    {
        m_ClassContentToDisplay->setPlainText("");
        return;
    }

    QString l_fullText;

    if( m_Methods.count() > 0U )
    {
        l_fullText = l_fullText + "Methods:\n";
    }

    for(unsigned short i_methods = 0U; i_methods < m_Methods.count(); i_methods++ )
    {
        l_fullText = l_fullText + m_Methods[i_methods] + "\n";
    }

    if( (m_Methods.count() != 0) && (m_Attributes.count() != 0) )
    {
        l_fullText = l_fullText + "\n\n";
    }

    if( m_Attributes.count() > 0U )
    {
        l_fullText = l_fullText + "Attributes:\n";
    }

    for( unsigned short i_attributes = 0U; i_attributes < m_Attributes.count(); i_attributes++ )
    {
        l_fullText = l_fullText + m_Attributes[i_attributes] + "\n";
    }

    m_ClassContentToDisplay->setPlainText(l_fullText);
}
