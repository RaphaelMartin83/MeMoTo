#include <QBrush>
#include <QtMath>
#include <QJsonArray>

#include <Engine/I_DiagramContainer.h>

#include <CommonGraphics/ConfigWidget.h>
#include <GraphicsItems/CompositionGraphicsItem.h>

#include <ConfigurationContexts/CompositionConfiguration.h>

const char* CompositionGraphicsItem::SERIALIZABLE_NAME = "Compositions";

static int compositionStartReduction = 20;
static int compositionEndReduction = 2;

CompositionGraphicsItem::CompositionGraphicsItem(
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
    this->setStartReduction(compositionStartReduction);
    this->setEndReduction(compositionEndReduction);
    CompositionGraphicsItem::refreshDisplay();
}

CompositionGraphicsItem::CompositionGraphicsItem(
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
    this->setStartReduction(compositionStartReduction);
    this->setEndReduction(compositionEndReduction);
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
    QJsonArray l_AttributesJson = p_JsonObject.find("AttributesNames")->toArray();
    m_AttributeNames.clear();
    for( QJsonArray::Iterator l_ArrayItem = l_AttributesJson.begin();
         l_ArrayItem < l_AttributesJson.end(); l_ArrayItem++ )
    {
        m_AttributeNames.append(l_ArrayItem->toString());
    }

    this->setStartReduction(compositionStartReduction);
    this->setEndReduction(compositionEndReduction);
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
    this->getConfig<CompositionConfiguration>().registerDiagram(this->getDiagramContainer());
    this->getConfig<CompositionConfiguration>().setListener(this->getSelectableType(), this->getID());

    // initializes module with parameters
    this->getConfig<CompositionConfiguration>().setAttributes(m_AttributeNames);

    this->getConfig<CompositionConfiguration>().setAutoRoute(!this->getPathIsForced());

    // Let's rock
    ConfigWidget::open(&this->getConfig<CompositionConfiguration>());
}
void CompositionGraphicsItem::applyConfiguration()
{
    this->setAttributeNames(this->getConfig<CompositionConfiguration>().getAttributes());
    this->setPathIsForced(!this->getConfig<CompositionConfiguration>().getAutoRoute());

    if( false == this->getPathIsForced() )
    {
        this->route(this->getPath().first(),
                    this->getPath().last());
    }

    this->getDiagramContainer()->changed(this);

    this->closeConfiguration();
}

// I_Serializable
QJsonObject CompositionGraphicsItem::toJson() const
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
void CompositionGraphicsItem::fromJson(const QJsonObject& p_Json)
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
