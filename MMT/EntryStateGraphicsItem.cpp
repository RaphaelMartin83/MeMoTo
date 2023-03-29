#include "EntryStateGraphicsItem.h"
#include "StateGraphicsItem.h"

#include <QBrush>

static const unsigned int DEFAULT_ENTRY_STATE_DIAMETER = static_cast<unsigned int>(GridReferential::getGridDistance(80));
static const char* ENTRY_STATE_CONNECTABLE_NAME = "EntryState";
const char* EntryStateGraphicsItem::SERIALIZABLE_NAME = "EntryStates";

EntryStateGraphicsItem::EntryStateGraphicsItem(const QPoint& p_Pos):
    I_Connectable(p_Pos)
  , m_isFullySelected(false)
  , m_SelectedHandlesDirection(eConnectionDirection_None)
  , m_Handles()
  , m_SelectedHandleForConnectionFrom(nullptr)
  , m_SelectedHandleForConnectionTo(nullptr)
{
    this->setDiameter(DEFAULT_ENTRY_STATE_DIAMETER);
    m_Circle = new QGraphicsEllipseItem();
    m_Circle->setBrush(Qt::green);
    this->addToGroup(m_Circle);

    EntryStateGraphicsItem::refreshDisplay();
}

EntryStateGraphicsItem::EntryStateGraphicsItem(const QJsonObject& p_JsonObject):
    I_Connectable(p_JsonObject)
  , m_isFullySelected(false)
  , m_SelectedHandlesDirection(eConnectionDirection_None)
  , m_Handles()
  , m_SelectedHandleForConnectionFrom(nullptr)
  , m_SelectedHandleForConnectionTo(nullptr)
{
    this->setDiameter(p_JsonObject.find("Diameter")->toInt());
    m_Circle = new QGraphicsEllipseItem();
    m_Circle->setBrush(Qt::green);
    this->addToGroup(m_Circle);

    EntryStateGraphicsItem::refreshDisplay();
}

EntryStateGraphicsItem::~EntryStateGraphicsItem()
{
    this->removeFromGroup(m_Circle);
    delete m_Circle;

    deleteHandles();
}

void EntryStateGraphicsItem::setDiameter(unsigned short p_Diameter)
{
    m_Diameter = GridReferential::getGridDistance(p_Diameter);
}

void EntryStateGraphicsItem::refreshDisplay()
{
    QRect l_CircleRect = QRect(this->getPos().x() - m_Diameter/2,
                      this->getPos().y() - m_Diameter/2,
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

void EntryStateGraphicsItem::selectAllHandles()
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        m_Handles[i_handles]->select(m_Handles[i_handles]->getHandleCoordinates());
    }

    m_isFullySelected = true;
}

void EntryStateGraphicsItem::selectHandlesOnSide(eConnectDirection p_Side)
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        if( p_Side == m_Handles[i_handles]->getHandlePosition() )
        {
            m_Handles[i_handles]->select(m_Handles[i_handles]->getHandleCoordinates());
        }
    }
}

void EntryStateGraphicsItem::setupHandles()
{
    QPoint l_HandleCoordinates;

    // Top handle
    l_HandleCoordinates.setX(this->getPos().x());
    l_HandleCoordinates.setY(this->getPos().y() - m_Diameter/2);
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Top));
    this->addToGroup(m_Handles.last());

    // Left handle
    l_HandleCoordinates.setX(this->getPos().x() - m_Diameter/2);
    l_HandleCoordinates.setY(this->getPos().y());
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Left));
    this->addToGroup(m_Handles.last());

    // Bottom handle
    l_HandleCoordinates.setX(this->getPos().x());
    l_HandleCoordinates.setY(this->getPos().y() + m_Diameter/2);
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Bottom));
    this->addToGroup(m_Handles.last());

    // Right handle
    l_HandleCoordinates.setX(this->getPos().x() + m_Diameter/2);
    l_HandleCoordinates.setY(this->getPos().y());
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Right));
    this->addToGroup(m_Handles.last());
}

void EntryStateGraphicsItem::deleteHandles()
{
    for(unsigned int i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        this->removeFromGroup(m_Handles[i_handles]);
        delete m_Handles[i_handles];
    }
    m_Handles.clear();
}

void EntryStateGraphicsItem::select()
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        m_Handles[i_handles]->select();
    }

    m_isFullySelected = true;
    m_SelectedHandlesDirection = eConnectionDirection_None;
}
void EntryStateGraphicsItem::select(QPoint p_Pos)
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

void EntryStateGraphicsItem::unselect()
{
    for( unsigned short i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        m_Handles[i_handles]->unselect();
    }

    m_isFullySelected = false;
    m_SelectedHandlesDirection = eConnectionDirection_None;
}

void EntryStateGraphicsItem::move(QPoint p_Pos)
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

void EntryStateGraphicsItem::translateTo(QPoint p_Pos)
{
    this->setPos(p_Pos);

    this->refreshDisplay();
}

void EntryStateGraphicsItem::resizeTo(QPoint p_Pos)
{
    // Depending on the direction of the selected handle, resize in different directions
    switch(m_SelectedHandlesDirection)
    {
    case eConnectionDirection_Left:
    {
        // Modifies size and position
        this->setDiameter(2*(this->getPos().x() - p_Pos.x()));
        if( m_Diameter < DEFAULT_ENTRY_STATE_DIAMETER )
        {
            m_Diameter = DEFAULT_ENTRY_STATE_DIAMETER;
        }
        break;
    }
    case eConnectionDirection_Right:
    {
        this->setDiameter(2*(p_Pos.x() - this->getPos().x()));
        if( m_Diameter < DEFAULT_ENTRY_STATE_DIAMETER )
        {
            m_Diameter = DEFAULT_ENTRY_STATE_DIAMETER;
        }
        break;
    }
    case eConnectionDirection_Top:
    {
        this->setDiameter(2*(this->getPos().y() - p_Pos.y()));
        if( m_Diameter < DEFAULT_ENTRY_STATE_DIAMETER )
        {
            m_Diameter = DEFAULT_ENTRY_STATE_DIAMETER;
        }
        break;
    }
    case eConnectionDirection_Bottom:
    {
        this->setDiameter(2*(p_Pos.y() - this->getPos().y()));
        if( m_Diameter < DEFAULT_ENTRY_STATE_DIAMETER )
        {
            m_Diameter = DEFAULT_ENTRY_STATE_DIAMETER;
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

bool EntryStateGraphicsItem::isItYou(QPoint p_Pos) const
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

QString EntryStateGraphicsItem::getConnectableType() const
{
    return ENTRY_STATE_CONNECTABLE_NAME;
}
const QList<HandleGraphicsItem*>& EntryStateGraphicsItem::getConnectableHandles() const
{
    return m_Handles;
}
QRect EntryStateGraphicsItem::getCollisionRectangle() const
{
    return QRect(this->getPos().x() - m_Diameter/2, this->getPos().y() - m_Diameter/2, m_Diameter, m_Diameter);
}
QString EntryStateGraphicsItem::getConnectableName() const
{
    return QString("EntryState" + this->getID().toString());
}

QJsonObject EntryStateGraphicsItem::toJson() const
{
    QJsonObject l_MyJson = I_Connectable::toJson();

    l_MyJson.insert("Diameter", m_Diameter);
    l_MyJson.insert("Name", m_Name);

    return l_MyJson;
}

void EntryStateGraphicsItem::fromJson(const QJsonObject& p_Json)
{
    I_Connectable::fromJson(p_Json);

    this->setDiameter(p_Json.find("Diameter")->toInt());
    m_Name = p_Json.find("Name")->toString();

    this->refreshDisplay();
}

QString EntryStateGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}

bool EntryStateGraphicsItem::isFullySelected() const
{
    return m_isFullySelected;
}

QPoint EntryStateGraphicsItem::getFocusPosition() const
{
    return this->getPos();
}

QString EntryStateGraphicsItem::getSelectableType() const
{
    return SERIALIZABLE_NAME;
}
QStringList EntryStateGraphicsItem::getSearchFields() const
{
    return QStringList("ID");
}
QString EntryStateGraphicsItem::getDataFromField(const QString& p_FieldName) const
{
    QString l_Ret;

    if( "ID" == p_FieldName )
    {
        l_Ret = this->getID().toString();
    }

    return l_Ret;
}
