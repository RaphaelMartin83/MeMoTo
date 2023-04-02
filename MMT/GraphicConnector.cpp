#include <QPen>
#include <QtMath>

#include "GraphicConnector.h"
#include "StackingDefinitions.h"
#include "I_ConnectableContainer.h"

static const unsigned short GRAPHIC_CONNECTOR_THICKNESS = 4U;
static const unsigned short GRAPHIC_CONNECTOR_SELECTION_THICKNESS = 16U;
static const Qt::GlobalColor GRAPHIC_CONNECTOR_COLOR = Qt::black;

GraphicConnector::GraphicConnector(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container):
    Connector(p_ConnectFrom, p_ConnectTo, p_fromPoint, p_toPoint, p_Container)
  , m_Lines()
  , m_Handles()
  , m_SelectedHandleID(-1)
  , m_isFullySelected(false)
{
    // All Connectors are on the highest plan possible
    this->setPlan(0U);
    GraphicConnector::refreshDisplay();
}

GraphicConnector::GraphicConnector(
            const I_Connectable* p_ConnectFrom,
            const I_Connectable* p_ConnectTo,
            const QPoint& p_fromPoint,
            const QPoint& p_toPoint,
            I_ConnectableContainer* p_Container,
            const QList<QPoint>& p_ForcedPath):
    Connector(p_ConnectFrom, p_ConnectTo, p_fromPoint, p_toPoint, p_Container, p_ForcedPath)
  , m_Lines()
  , m_Handles()
  , m_SelectedHandleID(-1)
  , m_isFullySelected(false)
{
    this->setPlan(0U);
    GraphicConnector::refreshDisplay();
}

GraphicConnector::GraphicConnector(const QJsonObject& p_JsonObject,
                                 I_ConnectableContainer* p_Container):
    Connector(p_JsonObject, p_Container)
  , m_Lines()
  , m_Handles()
  , m_SelectedHandleID(-1)
  , m_isFullySelected(false)
{
    this->setPlan(0U);
    GraphicConnector::refreshDisplay();
}

void GraphicConnector::route(QPoint p_From, QPoint p_To)
{
    Connector::route(p_From, p_To);

    this->unselect();

    this->refreshDisplay();
}

void GraphicConnector::reroute()
{
    Connector::reroute();

    this->unselect();

    this->refreshDisplay();
}

void GraphicConnector::clear()
{
    for( unsigned short i_lines=0U; i_lines<m_Lines.count(); i_lines++ )
    {
        this->removeFromGroup(m_Lines[i_lines]);
        delete m_Lines[i_lines];
    }
    m_Lines.clear();

    for( unsigned short i_handles=0U; i_handles < m_Handles.count(); i_handles++ )
    {
        this->removeFromGroup(m_Handles[i_handles]);
        delete m_Handles[i_handles];
    }
    m_Handles.clear();
}

void GraphicConnector::setupLines()
{
    if( 1 < this->getPath().count() )
    {
        for(unsigned short i_points = 0U; i_points < this->getPath().count() - 1U; i_points++ )
        {
            m_Lines.append(new QGraphicsLineItem(QLineF(this->getPath()[i_points], this->getPath()[i_points + 1U])));
            m_Lines.last()->setPen(QPen(GRAPHIC_CONNECTOR_COLOR, GRAPHIC_CONNECTOR_THICKNESS));
            this->addToGroup(m_Lines.last());
        }
    }
    else
    {
        // Let's admit it, if the connector has less than 2 points, it's my bad.
    }
}

void GraphicConnector::setupHandles()
{
    // Creates all handles of the transition
    for(unsigned short i_Points = 0U;
        i_Points < this->getPath().count(); i_Points++ )
    {
        m_Handles.append(new HandleGraphicsItem(this->getPath()[i_Points], eConnectionDirection_None));
        this->addToGroup(m_Handles.last());
        if( m_SelectedHandleID == i_Points )
        {
            m_Handles.last()->select(m_Handles.last()->getHandleCoordinates());
        }
    }
}

void GraphicConnector::deleteHandles()
{
    for(unsigned int i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        this->removeFromGroup(m_Handles[i_handles]);
        delete m_Handles[i_handles];
    }
    m_Handles.clear();
}

void GraphicConnector::refreshDisplay()
{
    this->clear();
    this->setupLines();
    this->deleteHandles();
    this->setupHandles();

    // Set a huge Z value so that transitions are always visible no matter what
    this->setZValue(CONNECTIONS_Z_DEPTH);
}

void GraphicConnector::selectAllHandles()
{
    for(unsigned short i_handles = 0U;
        i_handles < m_Handles.count(); i_handles++)
    {
        m_Handles[i_handles]->select(m_Handles[i_handles]->getHandleCoordinates());
    }
}

qreal GraphicConnector::getLastLineAngle() const
{
    return qDegreesToRadians(m_Lines.last()->line().angle());
}

void GraphicConnector::setLinesPen(QPen p_Pen)
{
    for( unsigned short i_lines = 0U; i_lines < m_Lines.count(); i_lines++ )
    {
        m_Lines[i_lines]->setPen(p_Pen);
    }
}

unsigned short GraphicConnector::getThickness() const
{
    return GRAPHIC_CONNECTOR_THICKNESS;
}

void GraphicConnector::select()
{
    this->selectAllHandles();
    m_isFullySelected = true;
}
void GraphicConnector::select(QPoint p_Pos)
{
    // First, check if it's a handle
    bool l_isFound = false;
    m_SelectedHandleID = -1;
    m_isFullySelected = false;

    for(unsigned short i_handles = 0U;
        (i_handles<m_Handles.count()) && (false == l_isFound);
        i_handles++ )
    {
        if( true == m_Handles[i_handles]->isItYou(p_Pos) )
        {
            l_isFound = true;
            m_Handles[i_handles]->select(p_Pos);
            m_SelectedHandleID = i_handles;
        }
    }

    QGraphicsLineItem l_tmpThickLine;
    QLineF l_tmpLine;
    for(unsigned short i_lines = 0U;
        (i_lines < m_Lines.count()) && (false == l_isFound);
        i_lines++)
    {
        // Uses a fake line to avoid computing
        l_tmpLine = m_Lines[i_lines]->line();
        l_tmpThickLine.setLine(l_tmpLine);
        l_tmpThickLine.setPen(QPen(GRAPHIC_CONNECTOR_COLOR, GRAPHIC_CONNECTOR_SELECTION_THICKNESS));
        if( true == l_tmpThickLine.contains(p_Pos) )
        {
            l_isFound = true;
            this->select();
        }
    }
}
void GraphicConnector::unselect()
{
    if( -1 == m_SelectedHandleID )
    {
        for(unsigned short i_handles = 0U;
            i_handles < m_Handles.count(); i_handles++)
        {
            m_Handles[i_handles]->unselect();
        }
    }
    else
    {
        m_Handles[m_SelectedHandleID]->unselect();
        m_SelectedHandleID = -1;
    }

    m_isFullySelected = false;
}
void GraphicConnector::move(QPoint p_Pos)
{
    if( (false == this->isFullySelected()) &&
            (m_SelectedHandleID == 0) )
    {
        // The first handle is selected
        I_Connectable* l_NewConnectable =
            this->getContainer()->getConnectable(p_Pos);
        if( (nullptr != l_NewConnectable) &&
                (eConnectionDirection_None != l_NewConnectable->getConnectDirectionFromPos(p_Pos)) )
        {
            this->setConnectFrom(l_NewConnectable);
            this->setConnectionFromDirection(
                l_NewConnectable->getConnectDirectionFromPos(p_Pos));
            this->setConnectionFromID(
                l_NewConnectable->getConnectionIDFromPos(p_Pos));
            this->reroute();
        }
    }
    else if( ( false == this->isFullySelected()) &&
             (m_SelectedHandleID == (m_Handles.count()-1)) )
    {
        // The last handle is selected
        I_Connectable* l_NewConnectable =
            this->getContainer()->getConnectable(p_Pos);
        if( (nullptr != l_NewConnectable) &&
                (eConnectionDirection_None != l_NewConnectable->getConnectDirectionFromPos(p_Pos)) )
        {
            this->setConnectTo(l_NewConnectable);
            this->setConnectionToDirection(
                l_NewConnectable->getConnectDirectionFromPos(p_Pos));
            this->setConnectionToID(
                l_NewConnectable->getConnectionIDFromPos(p_Pos));
            this->reroute();
        }
    }
    else if( ( false == this->isFullySelected()) &&
             (m_SelectedHandleID > 0) &&
             (m_SelectedHandleID < (m_Handles.count() - 1)) )
    {
        this->setPath(m_SelectedHandleID, p_Pos);
        this->refreshDisplay();
    }
    this->unselect();
}
QPoint GraphicConnector::getPos() const
{
    QPoint l_MiddlePoint;
    l_MiddlePoint.setX((this->getPath().first().x() + this->getPath().last().x())/2);
    l_MiddlePoint.setY((this->getPath().first().y() + this->getPath().last().y())/2);
    return l_MiddlePoint;
}
bool GraphicConnector::isItYou(QPoint p_Pos) const
{
    bool l_ret = false;

    for( unsigned short i_handles = 0U;
         (i_handles<m_Handles.count()) && (false == l_ret); i_handles++ )
    {
        l_ret = m_Handles[i_handles]->isItYou(p_Pos);
    }

    QGraphicsLineItem l_tmpThickLine;
    QLineF l_tmpLine;
    for( unsigned short i_lines = 0U;
         (i_lines < m_Lines.count()) && (false == l_ret); i_lines++ )
    {
        // Uses a fake line to avoid computing
        l_tmpLine = m_Lines[i_lines]->line();
        l_tmpThickLine.setLine(l_tmpLine);
        l_tmpThickLine.setPen(QPen(GRAPHIC_CONNECTOR_COLOR, GRAPHIC_CONNECTOR_SELECTION_THICKNESS));
        l_ret = l_tmpThickLine.contains(p_Pos);
    }

    return l_ret;
}

bool GraphicConnector::isSelected() const
{
    return (isFullySelected() || (-1 != m_SelectedHandleID));
}
bool GraphicConnector::isFullySelected() const
{
    return m_isFullySelected;
}

QPoint GraphicConnector::getFocusPosition() const
{
    return this->getPath().first();
}
