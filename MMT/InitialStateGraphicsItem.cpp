#include "InitialStateGraphicsItem.h"
#include "ConnectionDefinitions.h"
#include "EntryStateGraphicsItem.h"
#include "GridReferential.h"

#include <QBrush>

static const unsigned int DEFAULT_INIT_STATE_DIAMETER = static_cast<unsigned int>(GridReferential::getGridDistance(80));
static const unsigned short DEFAULT_HANDLES_DIAMETER = 8U;

static const char* INITIAL_STATE_CONNECTABLE_NAME = "InitialState";
const char* InitialStateGraphicsItem::SERIALIZABLE_NAME = "InitialStates";

static unsigned long long s_InitialStateUID = 0U;

InitialStateGraphicsItem::InitialStateGraphicsItem(QPointF p_Pos):
  m_isFullySelected(false)
  , m_SelectedHandlesDirection(eConnectionDirection_None)
  , m_Handles()
  , m_SelectedHandleForConnectionFrom(nullptr)
  , m_SelectedHandleForConnectionTo(nullptr)
  , m_Name()
{
    this->setID(s_InitialStateUID);
    s_InitialStateUID++;

    this->setDiameter(DEFAULT_INIT_STATE_DIAMETER);
    this->setPos(p_Pos);

    m_Circle = new QGraphicsEllipseItem();
    m_Circle->setBrush(Qt::blue);
    this->addToGroup(m_Circle);

    InitialStateGraphicsItem::refreshDisplay();
}

InitialStateGraphicsItem::InitialStateGraphicsItem(const QJsonObject& p_JsonObject):
    InitialStateGraphicsItem(QPointF(p_JsonObject.find("PositionX")->toDouble(),
                                   p_JsonObject.find("PositionY")->toDouble()))
{
    this->setID(p_JsonObject.find("ID")->toInt());
    s_InitialStateUID--;
    if(this->getID() >= s_InitialStateUID)
    {
        s_InitialStateUID = this->getID() + 1U;
    }
    this->setDiameter(p_JsonObject.find("Diameter")->toInt());

    InitialStateGraphicsItem::refreshDisplay();
}

InitialStateGraphicsItem::~InitialStateGraphicsItem()
{
    this->removeFromGroup(m_Circle);
    delete m_Circle;

    deleteHandles();
}

QPoint InitialStateGraphicsItem::getPos() const
{
    return m_Pos;
}
void InitialStateGraphicsItem::setPos(const QPoint& p_Pos)
{
    m_Pos = GridReferential::getPointOnGrid(p_Pos);
    this->refreshDisplay();
}
void InitialStateGraphicsItem::setPos(const QPointF& p_Pos)
{
    QPoint l_TempPoint;
    l_TempPoint.setX(p_Pos.x());
    l_TempPoint.setY(p_Pos.y());
    m_Pos = GridReferential::getPointOnGrid(l_TempPoint);
}
void InitialStateGraphicsItem::setDiameter(unsigned short p_Diameter)
{
    m_Diameter = GridReferential::getGridDistance(p_Diameter);
}

void InitialStateGraphicsItem::selectAllHandles()
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        m_Handles[i_handles]->select(m_Handles[i_handles]->getHandleCoordinates());
    }

    m_isFullySelected = true;
}

void InitialStateGraphicsItem::refreshDisplay()
{
    QRect l_CircleRect = QRect(m_Pos.x() - m_Diameter/2,
                      m_Pos.y() - m_Diameter/2,
                      m_Diameter, m_Diameter);
    m_Circle->setRect(l_CircleRect);

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

void InitialStateGraphicsItem::selectHandlesOnSide(eConnectDirection p_Side)
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        if( p_Side == m_Handles[i_handles]->getHandlePosition() )
        {
            m_Handles[i_handles]->select(m_Handles[i_handles]->getHandleCoordinates());
        }
    }
}

void InitialStateGraphicsItem::setupHandles()
{
    QPoint l_HandleCoordinates;

    // Top handle
    l_HandleCoordinates.setX(m_Pos.x());
    l_HandleCoordinates.setY(m_Pos.y() - m_Diameter/2);
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Top));
    this->addToGroup(m_Handles.last());

    // Left handle
    l_HandleCoordinates.setX(m_Pos.x() - m_Diameter/2);
    l_HandleCoordinates.setY(m_Pos.y());
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Left));
    this->addToGroup(m_Handles.last());

    // Bottom handle
    l_HandleCoordinates.setX(m_Pos.x());
    l_HandleCoordinates.setY(m_Pos.y() + m_Diameter/2);
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Bottom));
    this->addToGroup(m_Handles.last());

    // Right handle
    l_HandleCoordinates.setX(m_Pos.x() + m_Diameter/2);
    l_HandleCoordinates.setY(m_Pos.y());
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Right));
    this->addToGroup(m_Handles.last());
}

void InitialStateGraphicsItem::deleteHandles()
{
    for(unsigned int i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        this->removeFromGroup(m_Handles[i_handles]);
        delete m_Handles[i_handles];
    }
    m_Handles.clear();
}

void InitialStateGraphicsItem::select()
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        m_Handles[i_handles]->select();
    }

    m_isFullySelected = true;
    m_SelectedHandlesDirection = eConnectionDirection_None;
}
void InitialStateGraphicsItem::select(QPoint p_Pos)
{
    bool l_found = false;
    m_isFullySelected = false;

    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        if( true == m_Handles[i_handles]->isItYou(p_Pos) )
        {
            m_Handles[i_handles]->select(p_Pos);
            l_found = true;
            m_SelectedHandlesDirection = m_Handles[i_handles]->getHandlePosition();
        }
        else
        {
            m_Handles[i_handles]->unselect();
        }
    }

    if( false == l_found && this->m_Circle->contains(p_Pos) )
    {
        for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
        {
            m_Handles[i_handles]->select(p_Pos);
        }

        m_isFullySelected = true;
        m_SelectedHandlesDirection = eConnectionDirection_None;
    }
}

void InitialStateGraphicsItem::unselect()
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        m_Handles[i_handles]->unselect();
    }

    m_isFullySelected = false;
    m_SelectedHandlesDirection = eConnectionDirection_None;
}

void InitialStateGraphicsItem::move(QPoint p_Pos)
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

void InitialStateGraphicsItem::translateTo(QPoint p_Pos)
{
    this->setPos(p_Pos);

    this->refreshDisplay();
}

void InitialStateGraphicsItem::resizeTo(QPoint p_Pos)
{
    // Depending on the direction of the selected handle, resize in different directions
    switch(m_SelectedHandlesDirection)
    {
    case eConnectionDirection_Left:
    {
        // Modifies size and position
        this->setDiameter(2*(m_Pos.x() - p_Pos.x()));
        if( m_Diameter < DEFAULT_INIT_STATE_DIAMETER )
        {
            this->setDiameter(DEFAULT_INIT_STATE_DIAMETER);
        }
        break;
    }
    case eConnectionDirection_Right:
    {
        this->setDiameter(2*(p_Pos.x() - m_Pos.x()));
        if( m_Diameter < DEFAULT_INIT_STATE_DIAMETER )
        {
            this->setDiameter(DEFAULT_INIT_STATE_DIAMETER);
        }
        break;
    }
    case eConnectionDirection_Top:
    {
        this->setDiameter(2*(m_Pos.y() - p_Pos.y()));
        if( m_Diameter < DEFAULT_INIT_STATE_DIAMETER )
        {
            this->setDiameter(DEFAULT_INIT_STATE_DIAMETER);
        }
        break;
    }
    case eConnectionDirection_Bottom:
    {
        this->setDiameter(2*(p_Pos.y() - m_Pos.y()));
        if( m_Diameter < DEFAULT_INIT_STATE_DIAMETER )
        {
            this->setDiameter(DEFAULT_INIT_STATE_DIAMETER);
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

QString InitialStateGraphicsItem::getConnectableType() const
{
    return INITIAL_STATE_CONNECTABLE_NAME;
}
const QList<HandleGraphicsItem*>& InitialStateGraphicsItem::getConnectableHandles() const
{
    return m_Handles;
}
QRect InitialStateGraphicsItem::getCollisionRectangle() const
{
    return QRect(m_Pos.x() - m_Diameter/2, m_Pos.y() - m_Diameter/2, m_Diameter, m_Diameter);
}
QString InitialStateGraphicsItem::getConnectableName() const
{
    return QString("InitialState" + QString::number(this->getID()));
}

QJsonObject InitialStateGraphicsItem::toJson()
{
    QJsonObject l_MyJson;

    l_MyJson.insert("PositionX", m_Pos.x());
    l_MyJson.insert("PositionY", m_Pos.y());

    l_MyJson.insert("Diameter", m_Diameter);

    l_MyJson.insert("ID", static_cast<qint64>(this->getID()));

    l_MyJson.insert("Name", m_Name);

    return l_MyJson;
}

void InitialStateGraphicsItem::fromJson(QJsonObject p_Json)
{

}

QString InitialStateGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}

bool InitialStateGraphicsItem::isItYou(QPoint p_Pos) const
{
    bool l_isItMe = false;

    // First, check each handle perimeter
    for(unsigned short i_handles=0U;
        (i_handles < m_Handles.count()) && (false == l_isItMe);
        i_handles++ )
    {
        l_isItMe = m_Handles[i_handles]->isItYou(p_Pos);
    }

    l_isItMe = m_Circle->contains(p_Pos);

    return l_isItMe;
}

bool InitialStateGraphicsItem::isFullySelected() const
{
    return m_isFullySelected;
}
QPoint InitialStateGraphicsItem::getFocusPosition() const
{
    return this->getPos();
}
QString InitialStateGraphicsItem::getSelectableType() const
{
    return SERIALIZABLE_NAME;
}
QStringList InitialStateGraphicsItem::getSearchFields() const
{
    QStringList l_Ret;

    l_Ret.append("ID");

    return l_Ret;
}
QString InitialStateGraphicsItem::getDataFromField(const QString& p_FieldName) const
{
    QString l_Ret;

    if( "ID" == p_FieldName )
    {
        return QString::number(this->getID());
    }

    return l_Ret;
}
