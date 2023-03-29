#include "ChoiceGraphicsItem.h"
#include <QBrush>
#include <QTextDocument>
#include <QTextOption>

#include "ConnectionDefinitions.h"
#include "ConnectionDefinitions.h"
#include "ChoiceConfiguration.h"
#include "ConfigWidget.h"
#include "StateGraphicsItem.h"
#include "GridReferential.h"
#include "I_DiagramContainer.h"

static const unsigned short DEFAULT_CHOICE_WIDTH = 80U;
static const unsigned short DEFALUT_CHOICE_HEIGHT = 80U;
static const unsigned short CHOICE_TEXT_LABELS_HEIGHT = 15U;
static const unsigned short MIN_HANDLES_DISTANCE = 30U;
static const unsigned short SELECTION_FIELD_DIVISOR = 5U;

static const char* CHOICE_CONNECTABLE_TYPE = "Choice";
const char* ChoiceGraphicsItem::SERIALIZABLE_NAME = "Choices";

static ChoiceConfiguration* s_ConfigurationContext;

ChoiceGraphicsItem::ChoiceGraphicsItem(const QPoint& p_Pos):
    I_Connectable(p_Pos)
  , m_isFullySelected(false)
  , m_SelectedHandlesDirection(eConnectionDirection_None)
  , m_Handles()
  , m_SelectedHandle(nullptr)
  , m_SelectedHandleForConnectionFrom(nullptr)
  , m_SelectedHandleForConnectionTo(nullptr)
  , m_Name(QString("C"))
{
    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new ChoiceConfiguration();
    }

    m_Width = DEFAULT_CHOICE_WIDTH;
    m_Height = DEFALUT_CHOICE_HEIGHT;

    m_Polygon = new QGraphicsPolygonItem();
    m_Polygon->setBrush(Qt::green);
    this->addToGroup(m_Polygon);

    m_NameGraphicsItem = new QGraphicsTextItem(m_Name);
    this->addToGroup(m_NameGraphicsItem);

    ChoiceGraphicsItem::refreshDisplay();
}

ChoiceGraphicsItem::ChoiceGraphicsItem(const QJsonObject& p_JsonObject):
    I_Connectable(p_JsonObject)
  , m_isFullySelected(false)
  , m_SelectedHandlesDirection(eConnectionDirection_None)
  , m_Handles()
  , m_SelectedHandle(nullptr)
  , m_SelectedHandleForConnectionFrom(nullptr)
  , m_SelectedHandleForConnectionTo(nullptr)
  , m_Name(QString("C"))
{
    m_Name = p_JsonObject.find("Name")->toString();

    m_Width = p_JsonObject.find("Width")->toInt();
    m_Height = p_JsonObject.find("Height")->toInt();

    m_Polygon = new QGraphicsPolygonItem();
    m_Polygon->setBrush(Qt::green);
    this->addToGroup(m_Polygon);

    m_NameGraphicsItem = new QGraphicsTextItem(m_Name);
    this->addToGroup(m_NameGraphicsItem);

    ChoiceGraphicsItem::refreshDisplay();
}

ChoiceGraphicsItem::~ChoiceGraphicsItem()
{
    this->removeFromGroup(m_Polygon);
    delete m_Polygon;

    this->removeFromGroup(m_NameGraphicsItem);
    delete m_NameGraphicsItem;

    deleteHandles();
}

void ChoiceGraphicsItem::setName(const QString& p_Name)
{
    m_Name = p_Name;
    m_NameGraphicsItem->setPlainText(m_Name);
}
const QString& ChoiceGraphicsItem::getName() const
{
    return m_Name;
}

void ChoiceGraphicsItem::refreshDisplay()
{
    QPolygonF l_polyDef;
    l_polyDef << QPointF(this->getPos().x(), this->getPos().y() - m_Height/2) <<
                 QPointF(this->getPos().x() - m_Width/2, this->getPos().y()) <<
                 QPointF(this->getPos().x(), m_Height/2 + this->getPos().y()) <<
                 QPointF(m_Width/2 + this->getPos().x(), this->getPos().y());
    m_Polygon->setPolygon(l_polyDef);

    m_NameGraphicsItem->setPos(this->getPos().x() - m_Width/2, this->getPos().y() - CHOICE_TEXT_LABELS_HEIGHT);
    m_NameGraphicsItem->setTextWidth(m_Width);
    m_NameGraphicsItem->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    m_NameGraphicsItem->setPlainText(m_Name);

    this->deleteHandles();
    this->setupHandles();
    this->setZValue(getDepthFromPlan(this->getPlan()));

    if( true == m_isFullySelected )
    {
        this->selectAllHandles();
    }
    else
    {
        this->selectHandlesOnSide(m_SelectedHandlesDirection);
    }
}

void ChoiceGraphicsItem::selectAllHandles()
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        m_Handles[i_handles]->select(m_Handles[i_handles]->getHandleCoordinates());
    }

    m_isFullySelected = true;
}

void ChoiceGraphicsItem::selectHandlesOnSide(eConnectDirection p_Side)
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        if( p_Side == m_Handles[i_handles]->getHandlePosition() )
        {
            m_Handles[i_handles]->select(m_Handles[i_handles]->getHandleCoordinates());
        }
    }
}

void ChoiceGraphicsItem::translateTo(QPoint p_Pos)
{
    this->setPos(p_Pos);

    this->refreshDisplay();
}

void ChoiceGraphicsItem::resizeTo(QPoint p_Pos)
{
    // Depending on the direction of the selected handle, resize in different directions
    switch(m_SelectedHandlesDirection)
    {
    case eConnectionDirection_Left:
    {
        // Modifies size and position
        m_Width = 2*(this->getPos().x() - p_Pos.x());
        if( m_Width < DEFAULT_CHOICE_WIDTH )
        {
            m_Width = DEFAULT_CHOICE_WIDTH;
        }
        break;
    }
    case eConnectionDirection_Right:
    {
        m_Width = 2*(p_Pos.x() - this->getPos().x());
        if( m_Width < DEFAULT_CHOICE_WIDTH )
        {
            m_Width = DEFAULT_CHOICE_WIDTH;
        }
        break;
    }
    case eConnectionDirection_Top:
    {
        m_Height = 2*(this->getPos().y() - p_Pos.y());
        if( m_Height < DEFALUT_CHOICE_HEIGHT )
        {
            m_Height = DEFALUT_CHOICE_HEIGHT;
        }
        break;
    }
    case eConnectionDirection_Bottom:
    {
        m_Height = 2*(p_Pos.y() - this->getPos().y());
        if( m_Height < DEFALUT_CHOICE_HEIGHT )
        {
            m_Height = DEFALUT_CHOICE_HEIGHT;
        }
        break;
    }
    case eConnectionDirection_TopLeft:
    {
        break;
    }
    case eConnectionDirection_TopRight:
    {
        break;
    }
    case eConnectionDirection_BotLeft:
    {
        break;
    }
    case eConnectionDirection_BotRight:
    {
        break;
    }
    default:
    {
        break;
    }
    }
    this->refreshDisplay();
}

void ChoiceGraphicsItem::setupHandles()
{
    QPoint l_HandleCoordinates;

    qreal l_dx = m_Width/2;
    qreal l_dy = m_Height/2;
    qreal l_distance = qSqrt(qPow(l_dx, 2) + qPow(l_dy, 2));
    qreal l_nbOfHandles = qFloor(l_distance / static_cast<qreal>(MIN_HANDLES_DISTANCE));
    qreal l_XdistanceBetweenHandles = l_dx / l_nbOfHandles;

    // TopLeft
    qreal l_slope = -l_dy / l_dx;
    for(unsigned short i_Handles = 1U; i_Handles < l_nbOfHandles; i_Handles++ )
    {
        l_HandleCoordinates.setX(static_cast<int>(this->getPos().x() - l_dx + (l_XdistanceBetweenHandles * i_Handles)));
        l_HandleCoordinates.setY(static_cast<int>(this->getPos().y() + (l_slope*l_XdistanceBetweenHandles*i_Handles)));
        m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_TopLeft));
        this->addToGroup(m_Handles.last());
    }

    // TopRight
    l_slope = l_dy / l_dx;
    for(unsigned short i_Handles = 1U; i_Handles < l_nbOfHandles; i_Handles++ )
    {
        l_HandleCoordinates.setX(static_cast<int>(this->getPos().x() + (l_XdistanceBetweenHandles * i_Handles)));
        l_HandleCoordinates.setY(static_cast<int>(this->getPos().y() - l_dy + (l_slope*l_XdistanceBetweenHandles*i_Handles)));
        m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_TopRight));
        this->addToGroup(m_Handles.last());
    }

    // BotLeft
    l_slope = l_dy / l_dx;
    for(unsigned short i_Handles = 1U; i_Handles < l_nbOfHandles; i_Handles++ )
    {
        l_HandleCoordinates.setX(static_cast<int>(this->getPos().x() - l_dx + (l_XdistanceBetweenHandles * i_Handles)));
        l_HandleCoordinates.setY(static_cast<int>(this->getPos().y() + (l_slope*l_XdistanceBetweenHandles*i_Handles)));
        m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_BotLeft));
        this->addToGroup(m_Handles.last());
    }

    // BotRight
    l_slope = -l_dy / l_dx;
    for(unsigned short i_Handles = 1U; i_Handles < l_nbOfHandles; i_Handles++ )
    {
        l_HandleCoordinates.setX(static_cast<int>(this->getPos().x() + (l_XdistanceBetweenHandles * i_Handles)));
        l_HandleCoordinates.setY(static_cast<int>(this->getPos().y() + l_dy + (l_slope*l_XdistanceBetweenHandles*i_Handles)));
        m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_BotRight));
        this->addToGroup(m_Handles.last());
    }

    // Top handle
    l_HandleCoordinates.setX(this->getPos().x());
    l_HandleCoordinates.setY(this->getPos().y() - m_Height/2);
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Top));
    this->addToGroup(m_Handles.last());

    // Left handle
    l_HandleCoordinates.setX(this->getPos().x() - m_Width/2);
    l_HandleCoordinates.setY(this->getPos().y());
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Left));
    this->addToGroup(m_Handles.last());

    // Bottom handle
    l_HandleCoordinates.setX(this->getPos().x());
    l_HandleCoordinates.setY(this->getPos().y() + m_Height/2);
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Bottom));
    this->addToGroup(m_Handles.last());

    // Right handle
    l_HandleCoordinates.setX(this->getPos().x() + m_Width/2);
    l_HandleCoordinates.setY(this->getPos().y());
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Right));
    this->addToGroup(m_Handles.last());
}

void ChoiceGraphicsItem::deleteHandles()
{
    for(unsigned int i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        this->removeFromGroup(m_Handles[i_handles]);
        delete m_Handles[i_handles];
    }
    m_Handles.clear();
}

void ChoiceGraphicsItem::select()
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        m_Handles[i_handles]->select();
        m_SelectedHandle = m_Handles[i_handles];
        m_SelectedHandlesDirection = m_Handles[i_handles]->getHandlePosition();
    }

    m_isFullySelected = true;
    this->openConfiguration();
}
void ChoiceGraphicsItem::select(QPoint p_Pos)
{
    bool l_found = false;
    m_SelectedHandle = nullptr;

    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        if( true == m_Handles[i_handles]->isItYou(p_Pos) )
        {
            m_Handles[i_handles]->select(p_Pos);
            l_found = true;
            m_isFullySelected = false;
            m_SelectedHandle = m_Handles[i_handles];
            m_SelectedHandlesDirection = m_Handles[i_handles]->getHandlePosition();
        }
        else
        {
            m_Handles[i_handles]->unselect();
        }
    }

    if( false == l_found && this->m_Polygon->contains(p_Pos) )
    {
        eConnectDirection l_DirectionToSelect = eConnectionDirection_None;
        if( (p_Pos.x() < (this->getPos().x() - m_Width/SELECTION_FIELD_DIVISOR)) &&
                (p_Pos.y() > (this->getPos().y() - m_Height/SELECTION_FIELD_DIVISOR)) &&
                (p_Pos.y() < (this->getPos().y() + m_Height/SELECTION_FIELD_DIVISOR)) )
        {
            // Left corner
            l_DirectionToSelect = eConnectionDirection_Left;
            m_isFullySelected = false;
        }
        else if( (p_Pos.x() > (this->getPos().x() + m_Width/SELECTION_FIELD_DIVISOR)) &&
                (p_Pos.y() > (this->getPos().y() - m_Height/SELECTION_FIELD_DIVISOR)) &&
                (p_Pos.y() < (this->getPos().y() + m_Height/SELECTION_FIELD_DIVISOR)) )
        {
            // Right corner
            l_DirectionToSelect = eConnectionDirection_Right;
            m_isFullySelected = false;
        }
        else if( (p_Pos.y() > (this->getPos().y() + m_Height/SELECTION_FIELD_DIVISOR)) &&
                (p_Pos.x() > (this->getPos().x() - m_Width/SELECTION_FIELD_DIVISOR)) &&
                (p_Pos.x() < (this->getPos().x() + m_Width/SELECTION_FIELD_DIVISOR)) )
        {
            // Bot corner
            l_DirectionToSelect = eConnectionDirection_Bottom;
            m_isFullySelected = false;
        }
        else if( (p_Pos.y() < (this->getPos().y() - m_Height/SELECTION_FIELD_DIVISOR)) &&
                (p_Pos.x() > (this->getPos().x() - m_Width/SELECTION_FIELD_DIVISOR)) &&
                (p_Pos.x() < (this->getPos().x() + m_Width/SELECTION_FIELD_DIVISOR)) )
        {
            // Top corner
            l_DirectionToSelect = eConnectionDirection_Top;
            m_isFullySelected = false;
        }
        else
        {
            m_isFullySelected = true;
            this->openConfiguration();
        }
        for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
        {
            if( (eConnectionDirection_None == l_DirectionToSelect) ||
                    (l_DirectionToSelect == m_Handles[i_handles]->getHandlePosition()) )
            {
                m_Handles[i_handles]->select(p_Pos);
                m_SelectedHandle = m_Handles[i_handles];
                m_SelectedHandlesDirection = m_Handles[i_handles]->getHandlePosition();
            }
        }
    }
}

void ChoiceGraphicsItem::unselect()
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        m_Handles[i_handles]->unselect();
    }
    m_isFullySelected = false;
    m_SelectedHandle = nullptr;
    m_SelectedHandlesDirection = eConnectionDirection_None;

    this->closeConfiguration();
}

void ChoiceGraphicsItem::move(QPoint p_Pos)
{
    if( true == m_isFullySelected )
    {
        this->translateTo(p_Pos);
    }
    else
    {
        this->resizeTo(p_Pos);
    }
}

bool ChoiceGraphicsItem::isItYou(QPoint p_Pos) const
{
    bool l_isItMe = false;

    // First, check each handle perimeter
    for(unsigned short i_handles=0U;
        (i_handles < m_Handles.count()) && (false == l_isItMe);
        i_handles++ )
    {
        l_isItMe = m_Handles[i_handles]->isItYou(p_Pos);
    }

    l_isItMe = m_Polygon->contains(p_Pos);

    return l_isItMe;
}

QString ChoiceGraphicsItem::getConnectableType() const
{
    return CHOICE_CONNECTABLE_TYPE;
}
const QList<HandleGraphicsItem*>& ChoiceGraphicsItem::getConnectableHandles() const
{
    return m_Handles;
}
QRect ChoiceGraphicsItem::getCollisionRectangle() const
{
    return QRect(this->getPos().x() - m_Width/2, this->getPos().y() - m_Height/2, m_Width, m_Height);
}
QString ChoiceGraphicsItem::getConnectableName() const
{
    return m_Name;
}
bool ChoiceGraphicsItem::isFullySelected() const
{
    return m_isFullySelected;
}
QPoint ChoiceGraphicsItem::getFocusPosition() const
{
    return this->getPos();
}
QString ChoiceGraphicsItem::getSelectableType() const
{
    return SERIALIZABLE_NAME;
}
QStringList ChoiceGraphicsItem::getSearchFields() const
{
    QStringList l_Ret;

    l_Ret.append("Name");

    return l_Ret;
}
QString ChoiceGraphicsItem::getDataFromField(const QString& p_FieldName) const
{
    QString l_Ret;

    if( "Name" == p_FieldName )
    {
        l_Ret = m_Name;
    }

    return l_Ret;
}

void ChoiceGraphicsItem::openConfiguration()
{
    s_ConfigurationContext->registerDiagram(this->getDiagramContainer());
    s_ConfigurationContext->setListener(this->getSelectableType(), this->getID());

    // initializes module with parameters
    s_ConfigurationContext->setName(m_Name);

    // Let's rock
    ConfigWidget::open(s_ConfigurationContext);
}
void ChoiceGraphicsItem::closeConfiguration()
{
    ConfigWidget::close();
}
void ChoiceGraphicsItem::applyConfiguration()
{
    this->setName(s_ConfigurationContext->getName());

    this->getDiagramContainer()->changed(this);

    ConfigWidget::close();
}

QJsonObject ChoiceGraphicsItem::toJson() const
{
    QJsonObject l_MyJson = I_Connectable::toJson();

    l_MyJson.insert("Width", m_Width);
    l_MyJson.insert("Height", m_Height);
    l_MyJson.insert("Name", m_Name);

    return l_MyJson;
}

void ChoiceGraphicsItem::fromJson(const QJsonObject& p_Json)
{
    I_Connectable::fromJson(p_Json);

    m_Width = p_Json.find("Width")->toInt();
    m_Height = p_Json.find("Height")->toInt();
    m_Name = p_Json.find("Name")->toString();

    this->refreshDisplay();
}

QString ChoiceGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}
