#include <QPen>

#include <Engine/StackingDefinitions.h>
#include <Engine/I_DiagramContainer.h>

#include <CommonGraphics/ConfigWidget.h>
#include <GraphicsItems/TransitionGraphicsItem.h>

#include <ConfigurationContexts/TransitionConfiguration.h>

static const unsigned short TRANSITION_THICKNESS = 4U;
static const Qt::GlobalColor TRANSITION_COLOR = Qt::black;

static const unsigned short ARROW_WIDTH = 10U;
static const unsigned short ARROW_DEPTH = 10U;

static const float TRANSITION_LABEL_STRING_SIZE_FACTOR = 2.0F;

const char* TransitionGraphicsItem::SERIALIZABLE_NAME = "Transitions";

static int transitionStartReduction = 2;

TransitionGraphicsItem::TransitionGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container):
    GraphicConnector(p_ConnectFrom, p_ConnectTo, p_fromPoint, p_toPoint, p_Container)
  , m_Guard()
  , m_Event()
  , m_Action()
  , m_Stereotype()
  , m_Arrow()
  , m_LabelPosition()
  , m_LabelWidth()
  , m_Text(nullptr)
{
    if( this->getConnectFrom() == this->getConnectTo() )
    {
        this->setCurved(true);
    }

    this->setStartReduction(transitionStartReduction);
    TransitionGraphicsItem::refreshDisplay();
}
TransitionGraphicsItem::TransitionGraphicsItem(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container,
            const QList<QPoint>& p_ForcedPath):
    GraphicConnector(p_ConnectFrom, p_ConnectTo, p_fromPoint, p_toPoint, p_Container, p_ForcedPath)
  , m_Guard()
  , m_Event()
  , m_Action()
  , m_Stereotype()
  , m_Arrow()
  , m_LabelPosition()
  , m_LabelWidth()
  , m_Text(nullptr)
{
    if( this->getConnectFrom() == this->getConnectTo() )
    {
        this->setCurved(true);
    }

    this->setStartReduction(transitionStartReduction);
    TransitionGraphicsItem::refreshDisplay();
}
TransitionGraphicsItem::TransitionGraphicsItem(const QJsonObject& p_JsonObject,
                                               I_ConnectableContainer* p_Container):
    GraphicConnector(p_JsonObject, p_Container)
  , m_Arrow()
  , m_LabelPosition()
  , m_LabelWidth()
  , m_Text(nullptr)
{
    TransitionGraphicsItem::fromJson(p_JsonObject);

    if( this->getConnectFrom() == this->getConnectTo() )
    {
        this->setCurved(true);
    }

    this->setStartReduction(transitionStartReduction);
    TransitionGraphicsItem::refreshDisplay();
}

TransitionGraphicsItem::~TransitionGraphicsItem()
{
    // QGraphicsItemGroup deletes all registered children
    // Nothing to delete here then
}

void TransitionGraphicsItem::setEvent(const QString& p_Event)
{
    m_Event = p_Event;
    this->refreshDisplay();
}
const QString& TransitionGraphicsItem::getEvent() const
{
    return m_Event;
}

void TransitionGraphicsItem::setAction(const QString& p_Action)
{
    m_Action = p_Action;
    this->refreshDisplay();
}
const QString& TransitionGraphicsItem::getAction() const
{
    return m_Action;
}

void TransitionGraphicsItem::setGuard(const QString& p_Guard)
{
    m_Guard = p_Guard;
    this->refreshDisplay();
}
const QString& TransitionGraphicsItem::getGuard() const
{
    return m_Guard;
}

void TransitionGraphicsItem::setStereotype(const QString& p_Stereotype)
{
    m_Stereotype = p_Stereotype;
    this->refreshDisplay();
}
const QString& TransitionGraphicsItem::getStereotype() const
{
    return m_Stereotype;
}

void TransitionGraphicsItem::reroute()
{
    GraphicConnector::reroute();

    this->refreshDisplay();
}

void TransitionGraphicsItem::select()
{
    GraphicConnector::select();

    if( true == this->isFullySelected() )
    {
        this->openConfiguration();
    }
}
void TransitionGraphicsItem::select(QPoint p_Pos)
{
    GraphicConnector::select(p_Pos);

    if( true == this->isFullySelected() )
    {
        this->openConfiguration();
    }
}

void TransitionGraphicsItem::unselect()
{
    GraphicConnector::unselect();

    this->closeConfiguration();
}

QString TransitionGraphicsItem::getSelectableType() const
{
    return SERIALIZABLE_NAME;
}
QStringList TransitionGraphicsItem::getSearchFields() const
{
    QStringList l_Ret;

    l_Ret.append("Event");
    l_Ret.append("Action");
    l_Ret.append("Guard");
    l_Ret.append("Stereotype");

    return l_Ret;
}
QString TransitionGraphicsItem::getDataFromField(const QString& p_FieldName) const
{
    QString l_Ret;

    if( "Event" == p_FieldName )
    {
        l_Ret = this->getEvent();
    }
    else if( "Action" == p_FieldName )
    {
        l_Ret = this->getAction();
    }
    else if( "Guard" == p_FieldName )
    {
        l_Ret = this->getGuard();
    }
    else if( "Stereotype" == p_FieldName )
    {
        l_Ret = this->getStereotype();
    }

    return l_Ret;
}

void TransitionGraphicsItem::setupLabel()
{
    if( nullptr == m_Text)
    {
        m_Text = new TransitionGraphicsLabel();
        this->addToGroup(m_Text);
    }

    m_Text->setEvent(m_Event);
    m_Text->setAction(m_Action);
    m_Text->setGuard(m_Guard);
    m_Text->setStereotype(m_Stereotype);

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
    case eConnectionDirection_BotLeft:
    case eConnectionDirection_TopLeft:
    {
        m_LabelPosition.setX(this->getPath()[0].x() + qCos(this->getLastLineAngle())*m_Text->boundingRect().width());
        m_LabelPosition.setY(this->getPath()[0].y() - qSin(this->getLastLineAngle())*m_Text->boundingRect().width());
        m_Text->setRotation((-this->getLastLineAngle() / (2*M_PI) * 360) + 180);
        break;
    }
    case eConnectionDirection_BotRight:
    case eConnectionDirection_TopRight:
    {
        m_LabelPosition.setX(this->getPath()[0].x());
        m_LabelPosition.setY(this->getPath()[0].y());
        m_Text->setRotation(-this->getLastLineAngle() / (2*M_PI) * 360);
        break;
    }
    default:
        m_LabelPosition = this->getPath()[0];
        break;
    }
    m_Text->setPos(m_LabelPosition);
}

void TransitionGraphicsItem::refreshDisplay()
{
    this->clear();

    GraphicConnector::refreshDisplay();

    this->setupArrow();
    this->setupLabel();
}

void TransitionGraphicsItem::clear()
{
    GraphicConnector::clear();

    for( unsigned short i_arrow=0U; i_arrow < m_Arrow.count(); i_arrow++ )
    {
        this->removeFromGroup(m_Arrow[i_arrow]);
        delete m_Arrow[i_arrow];
    }
    m_Arrow.clear();
}

void TransitionGraphicsItem::setupArrow()
{
    static const unsigned short ARROW_DEPTH = 10U;
    static const unsigned short ARROW_WIDTH = 10U;

    // The middle point is the point in the middle of the tip, only for computation ease
    qreal l_lastLineAngle = this->getLastLineAngle();
    QPointF l_MiddlePoint;
    l_MiddlePoint.setX(this->getPath().last().x() - (qCos(l_lastLineAngle)*ARROW_DEPTH));
    l_MiddlePoint.setY(this->getPath().last().y() + (qSin(l_lastLineAngle)*ARROW_DEPTH));

    QList<QPointF> l_Points;
    // 3 points are needed to define the arrow
    l_Points.append(this->getPath().last()); // first point is end of line
    qreal l_currentPointAngle = l_lastLineAngle + (M_PI/2.0);
    l_Points.append(QPointF(l_MiddlePoint.x() + qCos(l_currentPointAngle)*ARROW_WIDTH,
                    l_MiddlePoint.y() - qSin(l_currentPointAngle)*ARROW_WIDTH));
    l_currentPointAngle += (M_PI);
    l_Points.append(QPointF(l_MiddlePoint.x() + qCos(l_currentPointAngle)*ARROW_WIDTH,
                    l_MiddlePoint.y() - qSin(l_currentPointAngle)*ARROW_WIDTH));

    for( unsigned short i_arrow=0U; i_arrow < m_Arrow.count(); i_arrow++ )
    {
        this->removeFromGroup(m_Arrow[i_arrow]);
        delete m_Arrow[i_arrow];
    }
    m_Arrow.clear();

    m_Arrow.append(new QGraphicsLineItem(QLineF(l_Points[0], l_Points[1])));
    m_Arrow.last()->setPen(QPen(TRANSITION_COLOR, TRANSITION_THICKNESS));
    this->addToGroup(m_Arrow.last());
    m_Arrow.append(new QGraphicsLineItem(QLineF(l_Points[0], l_Points[2])));
    m_Arrow.last()->setPen(QPen(TRANSITION_COLOR, TRANSITION_THICKNESS));
    this->addToGroup(m_Arrow.last());
}

void TransitionGraphicsItem::openConfiguration()
{
    this->getConfig<TransitionConfiguration>().registerDiagram(this->getDiagramContainer());
    this->getConfig<TransitionConfiguration>().setListener(this->getSelectableType(), this->getID());

    // initializes module with parameters
    this->getConfig<TransitionConfiguration>().setEvent(m_Event);
    this->getConfig<TransitionConfiguration>().setAction(m_Action);
    this->getConfig<TransitionConfiguration>().setGuard(m_Guard);
    this->getConfig<TransitionConfiguration>().setStereotype(m_Stereotype);
    this->getConfig<TransitionConfiguration>().setFromInfo(this->getConnectFrom()->getConnectableType(),
                                        this->getConnectFrom()->getConnectableName());
    this->getConfig<TransitionConfiguration>().setToInfo(this->getConnectTo()->getConnectableType(),
                                        this->getConnectTo()->getConnectableName());
    this->getConfig<TransitionConfiguration>().setAutoRoute(!this->getPathIsForced());
    this->getConfig<TransitionConfiguration>().setCurvedInfo(this->isCurved());

    // Let's rock
    ConfigWidget::open(&this->getConfig<TransitionConfiguration>());
}
void TransitionGraphicsItem::applyConfiguration()
{
    this->setEvent(this->getConfig<TransitionConfiguration>().getEvent());
    this->setAction(this->getConfig<TransitionConfiguration>().getAction());
    this->setGuard(this->getConfig<TransitionConfiguration>().getGuard());
    this->setStereotype(this->getConfig<TransitionConfiguration>().getStereotype());
    this->setPathIsForced(!this->getConfig<TransitionConfiguration>().getAutoRoute());
    this->setCurved(this->getConfig<TransitionConfiguration>().getCurvedInfo());

    if( false == this->getPathIsForced() )
    {
        this->route(this->getPath().first(),
                    this->getPath().last());
    }

    this->getDiagramContainer()->changed(this);

    this->closeConfiguration();
}

QJsonObject TransitionGraphicsItem::toJson() const
{
    QJsonObject l_MyJson = GraphicConnector::toJson();

    l_MyJson.insert("Guard", m_Guard);
    l_MyJson.insert("Event", m_Event);
    l_MyJson.insert("Action", m_Action);
    l_MyJson.insert("Stereotype", m_Stereotype);
    l_MyJson.insert("RouteType", this->isCurved()?"Curved":"Square");

    return l_MyJson;
}

void TransitionGraphicsItem::fromJson(const QJsonObject& p_Json)
{
    GraphicConnector::fromJson(p_Json);

    QJsonObject::const_iterator l_FoundObject;
    l_FoundObject = p_Json.find("Event");
    if( p_Json.end() != l_FoundObject )
        this->setEvent(l_FoundObject->toString());
    l_FoundObject = p_Json.find("Action");
    if( p_Json.end() != l_FoundObject )
        this->setAction(l_FoundObject->toString());
    l_FoundObject = p_Json.find("Guard");
    if( p_Json.end() != l_FoundObject )
        this->setGuard(l_FoundObject->toString());
    l_FoundObject = p_Json.find("Stereotype");
    if( p_Json.end() != l_FoundObject )
        this->setStereotype(l_FoundObject->toString());
    l_FoundObject = p_Json.find("RouteType");
    if( p_Json.end() != l_FoundObject )
        this->setCurved(l_FoundObject->toString() == "Curved");
}

QString TransitionGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}
