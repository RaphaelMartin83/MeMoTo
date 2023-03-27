#include <QBrush>
#include <QtMath>
#include <QJsonArray>

#include "CompositionGraphicsItem.h"
#include "ConfigWidget.h"
#include "CompositionConfiguration.h"
#include "I_DiagramContainer.h"

const char* CompositionGraphicsItem::SERIALIZABLE_NAME = "Compositions";

static CompositionConfiguration* s_ConfigurationContext;

static unsigned long long s_CompositionUID = 0U;

CompositionGraphicsItem::CompositionGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container):
    GraphicConnector(p_ConnectFrom, p_ConnectTo, p_fromPoint, p_toPoint, s_CompositionUID, p_Container)
  , m_AttributeNames()
  , m_Text(nullptr)
  , m_LabelPosition()
  , m_LabelWidth()
  , m_Tip(nullptr)
{
    s_CompositionUID++;

    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new CompositionConfiguration();
    }
    CompositionGraphicsItem::refreshDisplay();
}

CompositionGraphicsItem::CompositionGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container,
            const QList<QPoint>& p_ForcedPath):
    GraphicConnector(p_ConnectFrom, p_ConnectTo, p_fromPoint, p_toPoint, s_CompositionUID, p_Container, p_ForcedPath)
  , m_AttributeNames()
  , m_Text(nullptr)
  , m_LabelPosition()
  , m_LabelWidth()
  , m_Tip(nullptr)
{
    s_CompositionUID++;

    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new CompositionConfiguration();
    }
    CompositionGraphicsItem::refreshDisplay();
}

CompositionGraphicsItem::CompositionGraphicsItem(const QJsonObject& p_JsonObject,
                                               I_ConnectableContainer* p_Container):
    GraphicConnector(p_JsonObject, p_Container)
  , m_AttributeNames()
  , m_Text(nullptr)
  , m_LabelPosition()
  , m_LabelWidth()
  , m_Tip(nullptr)
{
    if(this->getID() >= s_CompositionUID)
    {
        s_CompositionUID = this->getID() + 1U;
    }

    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new CompositionConfiguration();
    }

    QJsonArray l_AttributesJson = p_JsonObject.find("AttributesNames")->toArray();
    m_AttributeNames.clear();
    for( QJsonArray::Iterator l_ArrayItem = l_AttributesJson.begin();
         l_ArrayItem < l_AttributesJson.end(); l_ArrayItem++ )
    {
        m_AttributeNames.append(l_ArrayItem->toString());
    }

    CompositionGraphicsItem::refreshDisplay();
}

void CompositionGraphicsItem::setAttributeNames(QList<QString> p_AttributeNames)
{
    m_AttributeNames = p_AttributeNames;
    this->setupLabel();
}

const QList<QString>& CompositionGraphicsItem::getAttributesNames() const
{
    return m_AttributeNames;
}

void CompositionGraphicsItem::reroute()
{
    GraphicConnector::reroute();

    this->refreshDisplay();
}

// I_Selectable overloads
void CompositionGraphicsItem::select()
{
    GraphicConnector::select();

    if( true == this->isFullySelected() )
    {
        this->openConfiguration();
    }
}
void CompositionGraphicsItem::select(QPoint p_Pos)
{
    GraphicConnector::select(p_Pos);

    if( true == this->isFullySelected() )
    {
        this->openConfiguration();
    }
}
void CompositionGraphicsItem::unselect()
{
    GraphicConnector::unselect();

    this->closeConfiguration();
}
QString CompositionGraphicsItem::getSelectableType() const
{
    return SERIALIZABLE_NAME;
}
QStringList CompositionGraphicsItem::getSearchFields() const
{
    QStringList l_Ret;

    l_Ret.append("Attributes names");

    return l_Ret;
}
QString CompositionGraphicsItem::getDataFromField(const QString& p_FieldName) const
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
void CompositionGraphicsItem::openConfiguration()
{
    s_ConfigurationContext->registerDiagram(this->getDiagramContainer());
    s_ConfigurationContext->setListener(this->getSelectableType(), this->getID());

    // initializes module with parameters
    s_ConfigurationContext->setAttributes(m_AttributeNames);

    s_ConfigurationContext->setAutoRoute(!this->getPathIsForced());

    // Let's rock
    ConfigWidget::open(s_ConfigurationContext);
}
void CompositionGraphicsItem::closeConfiguration()
{
    ConfigWidget::close();
}
void CompositionGraphicsItem::applyConfiguration()
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
QJsonObject CompositionGraphicsItem::toJson()
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
void CompositionGraphicsItem::fromJson(QJsonObject p_Json)
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
QString CompositionGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}

// Private methods
void CompositionGraphicsItem::setupTip()
{
    static const unsigned short ARROW_DEPTH = 10U;
    static const unsigned short ARROW_WIDTH = 10U;

    // The middle point is the point in the middle of the tip, only for computation ease
    qreal l_lastLineAngle = this->getLastLineAngle();
    QPointF l_MiddlePoint;
    l_MiddlePoint.setX(this->getPath().last().x() - (qCos(l_lastLineAngle)*ARROW_DEPTH));
    l_MiddlePoint.setY(this->getPath().last().y() + (qSin(l_lastLineAngle)*ARROW_DEPTH));

    QList<QPointF> l_Points;
    // The polynoma needs 4 points
    l_Points.append(this->getPath().last()); // first point is end of line
    qreal l_currentPointAngle = l_lastLineAngle + (M_PI/2.0);
    l_Points.append(QPointF(l_MiddlePoint.x() + qCos(l_currentPointAngle)*ARROW_WIDTH,
                    l_MiddlePoint.y() - qSin(l_currentPointAngle)*ARROW_WIDTH));
    l_currentPointAngle += (M_PI / 2.0);
    l_Points.append(QPointF(l_MiddlePoint.x() + qCos(l_currentPointAngle)*ARROW_DEPTH,
                    l_MiddlePoint.y() - qSin(l_currentPointAngle)*ARROW_DEPTH));
    l_currentPointAngle += (M_PI / 2.0);
    l_Points.append(QPointF(l_MiddlePoint.x() + qCos(l_currentPointAngle)*ARROW_WIDTH,
                    l_MiddlePoint.y() - qSin(l_currentPointAngle)*ARROW_WIDTH));

    QPolygonF l_polyCompose(l_Points);

    if( nullptr != m_Tip )
    {
        delete m_Tip;
    }
    m_Tip = new QGraphicsPolygonItem(l_polyCompose);
    m_Tip->setBrush(Qt::black);

    this->addToGroup(m_Tip);
}
void CompositionGraphicsItem::setupLabel()
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
void CompositionGraphicsItem::refreshDisplay()
{
    this->clear();

    GraphicConnector::refreshDisplay();

    this->setupTip();
    this->setupLabel();
}
void CompositionGraphicsItem::clear()
{
    GraphicConnector::clear();

    delete m_Tip;
    m_Tip = nullptr;
}
