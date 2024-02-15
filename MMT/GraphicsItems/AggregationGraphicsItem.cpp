#include <QBrush>
#include <QtMath>
#include <QJsonArray>

#include <Engine/I_DiagramContainer.h>

#include <CommonGraphics/ConfigWidget.h>

#include <ConfigurationContexts/AggregationConfiguration.h>

#include "AggregationGraphicsItem.h"

const char* AggregationGraphicsItem::SERIALIZABLE_NAME = "Aggregations";

static AggregationConfiguration* s_ConfigurationContext;

AggregationGraphicsItem::AggregationGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container):
    GraphicConnector(p_ConnectFrom, p_ConnectTo, p_fromPoint, p_toPoint, p_Container)
  , m_AttributeNames()
  , m_Text(nullptr)
  , m_LabelPosition()
  , m_LabelWidth()
  , m_Tip(nullptr)
{
    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new AggregationConfiguration();
    }
    AggregationGraphicsItem::refreshDisplay();
}

AggregationGraphicsItem::AggregationGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container,
            const QList<QPoint>& p_ForcedPath):
    GraphicConnector(p_ConnectFrom, p_ConnectTo, p_fromPoint, p_toPoint, p_Container, p_ForcedPath)
  , m_AttributeNames()
  , m_Text(nullptr)
  , m_LabelPosition()
  , m_LabelWidth()
  , m_Tip(nullptr)
{
    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new AggregationConfiguration();
    }
    AggregationGraphicsItem::refreshDisplay();
}

AggregationGraphicsItem::AggregationGraphicsItem(const QJsonObject& p_JsonObject,
                                               I_ConnectableContainer* p_Container):
    GraphicConnector(p_JsonObject, p_Container)
  , m_AttributeNames()
  , m_Text(nullptr)
  , m_LabelPosition()
  , m_LabelWidth()
  , m_Tip(nullptr)
{
    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new AggregationConfiguration();
    }

    QJsonArray l_AttributesJson = p_JsonObject.find("AttributesNames")->toArray();
    m_AttributeNames.clear();
    for( QJsonArray::Iterator l_ArrayItem = l_AttributesJson.begin();
         l_ArrayItem < l_AttributesJson.end(); l_ArrayItem++ )
    {
        m_AttributeNames.append(l_ArrayItem->toString());
    }

    AggregationGraphicsItem::refreshDisplay();
}

void AggregationGraphicsItem::setAttributeNames(QList<QString> p_AttributeNames)
{
    m_AttributeNames = p_AttributeNames;
    this->setupLabel();
}

const QList<QString>& AggregationGraphicsItem::getAttributesNames() const
{
    return m_AttributeNames;
}

void AggregationGraphicsItem::reroute()
{
    GraphicConnector::reroute();

    this->refreshDisplay();
}

// I_Selectable overloads
void AggregationGraphicsItem::select()
{
    GraphicConnector::select();

    if( true == this->isFullySelected() )
    {
        this->openConfiguration();
    }
}
void AggregationGraphicsItem::select(QPoint p_Pos)
{
    GraphicConnector::select(p_Pos);

    if( true == this->isFullySelected() )
    {
        this->openConfiguration();
    }
}
void AggregationGraphicsItem::unselect()
{
    GraphicConnector::unselect();

    this->closeConfiguration();
}
QString AggregationGraphicsItem::getSelectableType() const
{
    return SERIALIZABLE_NAME;
}
QStringList AggregationGraphicsItem::getSearchFields() const
{
    QStringList l_Ret;

    l_Ret.append("Attributes names");

    return l_Ret;
}
QString AggregationGraphicsItem::getDataFromField(const QString& p_FieldName) const
{
    QString l_Ret;

    if( "Attributes names" == p_FieldName )
    {
        for( unsigned short i_attributes = 0U; i_attributes < m_AttributeNames.count(); i_attributes++ )
        {
            l_Ret += m_AttributeNames[i_attributes] + ';';
        }
    }

    return l_Ret;
}

// I_Configurable
void AggregationGraphicsItem::openConfiguration()
{
    s_ConfigurationContext->registerDiagram(this->getDiagramContainer());
    s_ConfigurationContext->setListener(this->getSelectableType(), this->getID());

    // initializes module with parameters
    s_ConfigurationContext->setAttributes(m_AttributeNames);

    s_ConfigurationContext->setAutoRoute(!this->getPathIsForced());

    // Let's rock
    ConfigWidget::open(s_ConfigurationContext);
}
void AggregationGraphicsItem::closeConfiguration()
{
    ConfigWidget::close();
}
void AggregationGraphicsItem::applyConfiguration()
{
    this->setAttributeNames(s_ConfigurationContext->getAttributes());
    this->setPathIsForced(!s_ConfigurationContext->getAutoRoute());

    if( false == this->getPathIsForced() )
    {
        this->route(this->getPath().first(),
                    this->getPath().last());
    }

    this->getDiagramContainer()->changed(this);

    ConfigWidget::close();
}

// I_Serializable
QJsonObject AggregationGraphicsItem::toJson() const
{
    QJsonObject l_MyJson = GraphicConnector::toJson();

    QJsonArray l_AttributesNamesArray;
    for( unsigned short i_att = 0U; i_att < m_AttributeNames.count(); i_att++ )
    {
        l_AttributesNamesArray.append(m_AttributeNames[i_att]);
    }
    l_MyJson.insert("AttributesNames", l_AttributesNamesArray);

    return l_MyJson;
}
void AggregationGraphicsItem::fromJson(const QJsonObject& p_Json)
{
    GraphicConnector::fromJson(p_Json);

    QJsonArray l_AttributesJson = p_Json.find("AttributesNames")->toArray();
    m_AttributeNames.clear();
    for( QJsonArray::Iterator l_ArrayItem = l_AttributesJson.begin();
         l_ArrayItem < l_AttributesJson.end(); l_ArrayItem++ )
    {
        m_AttributeNames.append(l_ArrayItem->toString());
    }

    this->refreshDisplay();
}
QString AggregationGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}

// Private methods
void AggregationGraphicsItem::setupTip()
{
    static const unsigned short ARROW_DEPTH = 10U;
    static const unsigned short ARROW_WIDTH = 10U;

    // The middle point is the point in the middle of the tip, only for computation ease
    qreal l_firstLineAngle = this->getFirstAngle();
    QPointF l_MiddlePoint;
    l_MiddlePoint.setX(this->getPath().first().x() + (qCos(l_firstLineAngle)*ARROW_DEPTH));
    l_MiddlePoint.setY(this->getPath().first().y() - (qSin(l_firstLineAngle)*ARROW_DEPTH));

    QList<QPointF> l_Points;
    // The polynoma needs 4 points
    l_Points.append(this->getPath().first()); // first point is end of line
    qreal l_currentPointAngle = l_firstLineAngle + (M_PI/2.0);
    l_Points.append(QPointF(l_MiddlePoint.x() + qCos(l_currentPointAngle)*ARROW_WIDTH,
                    l_MiddlePoint.y() - qSin(l_currentPointAngle)*ARROW_WIDTH));
    l_currentPointAngle -= (M_PI / 2.0);
    l_Points.append(QPointF(l_MiddlePoint.x() + qCos(l_currentPointAngle)*ARROW_DEPTH,
                    l_MiddlePoint.y() - qSin(l_currentPointAngle)*ARROW_DEPTH));
    l_currentPointAngle -= (M_PI / 2.0);
    l_Points.append(QPointF(l_MiddlePoint.x() + qCos(l_currentPointAngle)*ARROW_WIDTH,
                    l_MiddlePoint.y() - qSin(l_currentPointAngle)*ARROW_WIDTH));

    QPolygonF l_polyCompose(l_Points);

    if( nullptr != m_Tip )
    {
        delete m_Tip;
    }
    m_Tip = new QGraphicsPolygonItem(l_polyCompose);
    m_Tip->setBrush(Qt::white);

    this->addToGroup(m_Tip);
}
void AggregationGraphicsItem::setupLabel()
{
    if( nullptr == m_Text)
    {
        m_Text = new QGraphicsTextItem();
        this->addToGroup(m_Text);
    }

    if( m_AttributeNames.count() != 0 )
    {
        if( m_AttributeNames.count() > 1 )
        {
            m_Text->setPlainText(m_AttributeNames[0] + ", ...");
        }
        else
        {
            m_Text->setPlainText(m_AttributeNames[0]);
        }
        m_Text->setDefaultTextColor(Qt::GlobalColor::darkGray);
    }

    switch(this->getConnectFrom()->getConnectDirectionFromPos(this->getPath()[0]))
    {
    case eConnectionDirection_Top:
    {
        m_LabelPosition.setX(this->getPath()[0].x());
        m_LabelPosition.setY(this->getPath()[0].y() - 30);
        break;
    }
    case eConnectionDirection_Bottom:
        m_LabelPosition = this->getPath()[0];
        break;
    case eConnectionDirection_Left:
    {
        m_LabelPosition.setX(this->getPath()[0].x() - m_Text->boundingRect().width());
        m_LabelPosition.setY(this->getPath()[0].y());
        break;
    }
    case eConnectionDirection_Right:
        m_LabelPosition = this->getPath()[0];
        break;
    default:
        m_LabelPosition = this->getPath()[0];
        break;
    }
    m_Text->setPos(m_LabelPosition);
}
void AggregationGraphicsItem::refreshDisplay()
{
    this->clear();

    GraphicConnector::refreshDisplay();

    this->setupTip();
    this->setupLabel();
}
void AggregationGraphicsItem::clear()
{
    GraphicConnector::clear();

    delete m_Tip;
    m_Tip = nullptr;
}
