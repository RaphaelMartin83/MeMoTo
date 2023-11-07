#include <QJsonArray>

#include "Connector.h"
#include "GridReferential.h"
#include "I_ConnectableContainer.h"

static const int CLEARANCE_FROM_CONNECTABLE = 50;

Connector::Connector(const I_Connectable* p_ConnectFrom,
              const I_Connectable* p_ConnectTo,
              const QPoint& p_fromPoint,
              const QPoint& p_toPoint,
              I_ConnectableContainer* p_Container):
    m_Path()
  , m_PathIsForced(false)
  , m_Container(p_Container)
  , m_ClearanceFromConnectables(CLEARANCE_FROM_CONNECTABLE)
{
    m_ConnectFrom = p_ConnectFrom;
    m_ConnectTo = p_ConnectTo;

    m_ConnectionFromDirection = m_ConnectFrom->getConnectDirectionFromPos(p_fromPoint);
    m_ConnectionToDirection = m_ConnectTo->getConnectDirectionFromPos(p_toPoint);

    m_ConnectionFromID = m_ConnectFrom->getConnectionIDFromPos(p_fromPoint);
    m_ConnectionToID = m_ConnectTo->getConnectionIDFromPos(p_toPoint);

    this->route(p_fromPoint, p_toPoint);
}

Connector::Connector(const I_Connectable* p_ConnectFrom,
              const I_Connectable* p_ConnectTo,
              const QPoint& p_fromPoint,
              const QPoint& p_toPoint,
              I_ConnectableContainer* p_Container,
              const QList<QPoint>& p_ForcedPath):
    m_Path()
  , m_PathIsForced(true)
  , m_Container(p_Container)
  , m_ClearanceFromConnectables(CLEARANCE_FROM_CONNECTABLE)
{
    m_ConnectFrom = p_ConnectFrom;
    m_ConnectTo = p_ConnectTo;

    m_ConnectionFromDirection = m_ConnectFrom->getConnectDirectionFromPos(p_fromPoint);
    m_ConnectionToDirection = m_ConnectTo->getConnectDirectionFromPos(p_toPoint);

    m_ConnectionFromID = m_ConnectFrom->getConnectionIDFromPos(p_fromPoint);
    m_ConnectionToID = m_ConnectTo->getConnectionIDFromPos(p_toPoint);

    m_Path.append(p_fromPoint);
    m_Path.append(p_toPoint);
    this->setPath(p_ForcedPath);
}

Connector::Connector(const QJsonObject& p_JsonObject,
                     I_ConnectableContainer* p_Container):
    I_Connector(p_JsonObject)
  , m_Path()
  , m_PathIsForced(false)
  , m_Container(p_Container)
  , m_ClearanceFromConnectables(CLEARANCE_FROM_CONNECTABLE)
{
    Connector::fromJson(p_JsonObject);
}

int Connector::getClearanceFromConnectables() const
{
    return m_ClearanceFromConnectables;
}
void Connector::setClearanceFromConnectables(int p_Clearance)
{
    m_ClearanceFromConnectables = p_Clearance;
}

// I_Serializable
QJsonObject Connector::toJson() const
{
    QJsonObject l_MyJson = I_Connector::toJson();

    l_MyJson.insert("ConnectFromType", this->getConnectFrom()->getConnectableType());
    l_MyJson.insert("ConnectFromID", this->getConnectFrom()->getID().toString());
    l_MyJson.insert("ConnectFromDirection", this->getConnectFrom()->getConnectDirectionFromPos(this->getPath()[0U]));
    l_MyJson.insert("ConnectFromNumber", this->getConnectFrom()->getConnectionIDFromPos(this->getPath()[0U]));

    l_MyJson.insert("ConnectToType", this->getConnectTo()->getConnectableType());
    l_MyJson.insert("ConnectToID", this->getConnectTo()->getID().toString());
    l_MyJson.insert("ConnectToDirection", this->getConnectTo()->getConnectDirectionFromPos(this->getPath().last()));
    l_MyJson.insert("ConnectToNumber", this->getConnectTo()->getConnectionIDFromPos(this->getPath().last()));

    if( true == m_PathIsForced )
    {
        // Write points between start and end
        QJsonArray l_PathJson;
        for( unsigned short i_Points = 1U; i_Points < (m_Path.count() - 1U); i_Points++ )
        {
            QJsonArray l_PointJson;
            l_PointJson.append(m_Path[i_Points].x());
            l_PointJson.append(m_Path[i_Points].y());
            l_PathJson.append(l_PointJson);
        }
        l_MyJson.insert("ForcedPath", l_PathJson);
    }

    return l_MyJson;
}

void Connector::fromJson(const QJsonObject& p_JsonObject)
{
    I_Connector::fromJson(p_JsonObject);

    Connector::setConnectFrom(m_Container->getConnectableFromTypeAndID(
                            p_JsonObject.find("ConnectFromType")->toString(),
                            QUuid::fromString(p_JsonObject.find("ConnectFromID")->toString())));
    Connector::setConnectTo(m_Container->getConnectableFromTypeAndID(
                            p_JsonObject.find("ConnectToType")->toString(),
                            QUuid::fromString(p_JsonObject.find("ConnectToID")->toString())));

    QPoint l_FromPoint = Connector::getConnectFrom()->getConnectionPosFromIDAndDirection(
                                    p_JsonObject.find("ConnectFromNumber")->toInt(),
                                    static_cast<eConnectDirection>(p_JsonObject.find("ConnectFromDirection")->toInt()));
    QPoint l_ToPoint = Connector::getConnectTo()->getConnectionPosFromIDAndDirection(
                                    p_JsonObject.find("ConnectToNumber")->toInt(),
                                    static_cast<eConnectDirection>(p_JsonObject.find("ConnectToDirection")->toInt()));
    QJsonArray l_PathJson;
    QJsonObject::const_iterator l_PathFound = p_JsonObject.find("ForcedPath");
    if( p_JsonObject.end() != l_PathFound )
    {
        l_PathJson = l_PathFound->toArray();
    }

    m_ConnectionFromDirection = m_ConnectFrom->getConnectDirectionFromPos(l_FromPoint);
    m_ConnectionToDirection = m_ConnectTo->getConnectDirectionFromPos(l_ToPoint);

    m_ConnectionFromID = m_ConnectFrom->getConnectionIDFromPos(l_FromPoint);
    m_ConnectionToID = m_ConnectTo->getConnectionIDFromPos(l_ToPoint);

    if( 0U == l_PathJson.count() )
    {
        // Path is not forced, needs route
        this->route(l_FromPoint, l_ToPoint);
    }
    else
    {
        // Path is forced, just copy it
        m_Path.clear();
        m_Path.append(l_FromPoint);
        for( unsigned short i_Points = 0U; i_Points < l_PathJson.count(); i_Points++ )
        {
            QPoint l_Point(l_PathJson[i_Points].toArray()[0U].toInt(),
                    l_PathJson[i_Points].toArray()[1U].toInt());
            this->setPath(i_Points + 1U, l_Point);
        }
        m_Path.append(l_ToPoint);
    }
}

const QList<QPoint>& Connector::getPath() const
{
    return m_Path;
}

void Connector::setPath(unsigned short p_index, QPoint p_Point)
{
    m_PathIsForced = true;
    if(m_Path.count() > p_index)
    {
        m_Path[p_index] = GridReferential::getPointOnGrid(p_Point);
    }
    else
    {
        m_Path.append(GridReferential::getPointOnGrid(p_Point));
    }
}
void Connector::setPath(const QList<QPoint>& p_Points)
{
    QPoint l_FromPt = this->getPath().first();
    QPoint l_ToPoint = this->getPath().last();

    m_Path.clear();

    this->setPath(0U, l_FromPt);
    unsigned short i_pt = 1U;
    for(; i_pt <= p_Points.count(); i_pt++)
    {
        this->setPath(i_pt, p_Points[i_pt - 1U]);
    }
    this->setPath(i_pt, l_ToPoint);
}

I_ConnectableContainer* Connector::getContainer()
{
    return m_Container;
}

const I_Connectable* const Connector::getConnectFrom() const
{
    return m_ConnectFrom;
}
void Connector::setConnectFrom(const I_Connectable* p_ConnectFrom)
{
    m_ConnectFrom = p_ConnectFrom;
}
const I_Connectable* const Connector::getConnectTo() const
{
    return m_ConnectTo;
}
void Connector::setConnectTo(const I_Connectable* p_ConnectTo)
{
    m_ConnectTo = p_ConnectTo;
}
eConnectDirection Connector::getConnectionFromDirection() const
{
    return m_ConnectionFromDirection;
}
void Connector::setConnectionFromDirection(eConnectDirection p_Direction)
{
    m_ConnectionFromDirection = p_Direction;
}
eConnectDirection Connector::getConnectionToDirection() const
{
    return m_ConnectionToDirection;
}
void Connector::setConnectionToDirection(eConnectDirection p_Direction)
{
    m_ConnectionToDirection = p_Direction;
}
unsigned short Connector::getConnectionFromID() const
{
    return m_ConnectionFromID;
}
void Connector::setConnectionFromID(unsigned short p_ID)
{
    m_ConnectionFromID = p_ID;
}
unsigned short Connector::getConnectionToID() const
{
    return m_ConnectionToID;
}
void Connector::setConnectionToID(unsigned short p_ID)
{
    m_ConnectionToID = p_ID;
}
bool Connector::getPathIsForced() const
{
    return m_PathIsForced;
}
void Connector::setPathIsForced(bool p_isForced)
{
    m_PathIsForced = p_isForced;
}

bool Connector::needReroute() const
{
    QPoint l_fromPoint = m_ConnectFrom->getConnectionPosFromIDAndDirection(m_ConnectionFromID, m_ConnectionFromDirection);
    QPoint l_toPoint = m_ConnectTo->getConnectionPosFromIDAndDirection(m_ConnectionToID, m_ConnectionToDirection);

    return (this->getPath()[0] != l_fromPoint) || (this->getPath().last() != l_toPoint);
}
void Connector::route(const QPoint& p_fromPoint, const QPoint& p_toPoint)
{
    m_Path.clear();
    m_PathIsForced = false;

    // First rough sorting of routing cases to divide algorithm
    if( (eConnectionDirection_Bottom == m_ConnectionFromDirection) && (eConnectionDirection_Top == m_ConnectionToDirection) )
    {
        if( (p_toPoint.y() - p_fromPoint.y()) > this->getClearanceFromConnectables() )
        {
            // They are foce to face
            this->routeFaceToFace(p_fromPoint, p_toPoint);
        }
        else
        {
            // They are back to back
            this->routeBackToBack(p_fromPoint, p_toPoint);
        }
    }
    else if( (eConnectionDirection_Top == m_ConnectionFromDirection) && (eConnectionDirection_Bottom == m_ConnectionToDirection) )
    {
        if( (p_fromPoint.y() - p_toPoint.y()) > this->getClearanceFromConnectables() )
        {
            this->routeFaceToFace(p_fromPoint, p_toPoint);
        }
        else
        {
            this->routeBackToBack(p_fromPoint, p_toPoint);
        }
    }
    else if( (eConnectionDirection_Right == m_ConnectionFromDirection) && (eConnectionDirection_Left == m_ConnectionToDirection) )
    {
        if( (p_toPoint.x() - p_fromPoint.x()) > this->getClearanceFromConnectables() )
        {
            this->routeFaceToFace(p_fromPoint, p_toPoint);
        }
        else
        {
            this->routeBackToBack(p_fromPoint, p_toPoint);
        }
    }
    else if( (eConnectionDirection_Left == m_ConnectionFromDirection) && (eConnectionDirection_Right == m_ConnectionToDirection) )
    {
        if( (p_fromPoint.x() - p_toPoint.x()) > this->getClearanceFromConnectables() )
        {
            this->routeFaceToFace(p_fromPoint, p_toPoint);
        }
        else
        {
            this->routeBackToBack(p_fromPoint, p_toPoint);
        }
    }
    else if( (eConnectionDirection_Bottom == m_ConnectionFromDirection) && (eConnectionDirection_Bottom == m_ConnectionToDirection) )
    {
        if( m_ConnectFrom == m_ConnectTo )
        {
            this->routeSideToSideSelf(p_fromPoint, p_toPoint);
        }
        else
        {
            if( (p_toPoint.y() - p_fromPoint.y()) > this->getClearanceFromConnectables() )
            {
                this->routeFaceToBack(p_fromPoint, p_toPoint);
            }
            else
            {
                this->routeBackToFace(p_fromPoint, p_toPoint);
            }
        }
    }
    else if( (eConnectionDirection_Top == m_ConnectionFromDirection) && (eConnectionDirection_Top == m_ConnectionToDirection) )
    {
        if( m_ConnectFrom == m_ConnectTo )
        {
            this->routeSideToSideSelf(p_fromPoint, p_toPoint);
        }
        else
        {
            if( (p_toPoint.y() - p_fromPoint.y()) > this->getClearanceFromConnectables() )
            {
                this->routeBackToFace(p_fromPoint, p_toPoint);
            }
            else
            {
                this->routeFaceToBack(p_fromPoint, p_toPoint);
            }
        }
    }
    else if( (eConnectionDirection_Right == m_ConnectionFromDirection) && (eConnectionDirection_Right == m_ConnectionToDirection) )
    {
        if( m_ConnectFrom == m_ConnectTo )
        {
            this->routeSideToSideSelf(p_fromPoint, p_toPoint);
        }
        else
        {
            if( (p_toPoint.x() - p_fromPoint.x()) > this->getClearanceFromConnectables() )
            {
                this->routeFaceToBack(p_fromPoint, p_toPoint);
            }
            else
            {
                this->routeBackToFace(p_fromPoint, p_toPoint);
            }
        }
    }
    else if( (eConnectionDirection_Left == m_ConnectionFromDirection) && (eConnectionDirection_Left == m_ConnectionToDirection) )
    {
        if( m_ConnectFrom == m_ConnectTo )
        {
            this->routeSideToSideSelf(p_fromPoint, p_toPoint);
        }
        else
        {
            if( (p_toPoint.x() - p_fromPoint.x()) > this->getClearanceFromConnectables() )
            {
                this->routeBackToFace(p_fromPoint, p_toPoint);
            }
            else
            {
                this->routeFaceToBack(p_fromPoint, p_toPoint);
            }
        }
    }
    else if( (eConnectionDirection_Bottom == m_ConnectionFromDirection) && (eConnectionDirection_Right == m_ConnectionToDirection ||
                                                                                 eConnectionDirection_Left == m_ConnectionToDirection ))
    {
        if( (p_toPoint.y() - p_fromPoint.y()) > this->getClearanceFromConnectables() )
        {
            this->routeFaceToSide(p_fromPoint, p_toPoint);
        }
        else
        {
            this->routeBackToSide(p_fromPoint, p_toPoint);
        }
    }
    else if( (eConnectionDirection_Top == m_ConnectionFromDirection) && (eConnectionDirection_Right == m_ConnectionToDirection ||
                                                                              eConnectionDirection_Left == m_ConnectionToDirection ))
    {
        if( (p_fromPoint.y() - p_toPoint.y()) > this->getClearanceFromConnectables() )
        {
            this->routeFaceToSide(p_fromPoint, p_toPoint);
        }
        else
        {
            this->routeBackToSide(p_fromPoint, p_toPoint);
        }
    }
    else if( (eConnectionDirection_Right == m_ConnectionFromDirection) && (eConnectionDirection_Top == m_ConnectionToDirection ||
                                                                                eConnectionDirection_Bottom == m_ConnectionToDirection ))
    {
        if( (p_toPoint.x() - p_fromPoint.x()) > this->getClearanceFromConnectables() )
        {
            this->routeFaceToSide(p_fromPoint, p_toPoint);
        }
        else
        {
            this->routeBackToSide(p_fromPoint, p_toPoint);
        }
    }
    else if( (eConnectionDirection_Left == m_ConnectionFromDirection) && (eConnectionDirection_Top == m_ConnectionToDirection ||
                                                                                eConnectionDirection_Bottom == m_ConnectionToDirection ))
    {
        if( (p_fromPoint.x() - p_toPoint.x()) > this->getClearanceFromConnectables() )
        {
            this->routeFaceToSide(p_fromPoint, p_toPoint);
        }
        else
        {
            this->routeBackToSide(p_fromPoint, p_toPoint);
        }
    }
    else
    {
        this->routeFaceToFace(p_fromPoint, p_toPoint);
    }
}

void Connector::reroute()
{
    QPoint l_fromPoint = m_ConnectFrom->getConnectionPosFromIDAndDirection(m_ConnectionFromID, m_ConnectionFromDirection);
    QPoint l_toPoint = m_ConnectTo->getConnectionPosFromIDAndDirection(m_ConnectionToID, m_ConnectionToDirection);

    if( true == m_PathIsForced )
    {
        // Compute displacement of from point and to point
        int l_fdx = l_fromPoint.x() - m_Path[0].x();
        int l_fdy = l_fromPoint.y() - m_Path[0].y();
        int l_tdx = l_toPoint.x() - m_Path.last().x();
        int l_tdy = l_toPoint.y() - m_Path.last().y();
        if( (l_fdx == l_tdx) && (l_fdy == l_tdy) )
        {
            // It's a translation
            for( unsigned int i_pt = 0U; i_pt < this->getPath().count(); i_pt++ )
            {
                QPoint l_tmpPt = this->getPath()[i_pt];
                l_tmpPt.setX(l_tmpPt.x() + l_fdx);
                l_tmpPt.setY(l_tmpPt.y() + l_fdy);
                this->setPath(i_pt, l_tmpPt);
            }
        }
        else
        {
            // It's not a translation, do what we can
            this->setPath(0U, l_fromPoint);
            this->setPath(this->getPath().count() - 1, l_toPoint);
        }
    }
    else
    {
        this->route(l_fromPoint, l_toPoint);
    }
}

void Connector::routeFaceToFace(const QPoint& p_fromPoint, const QPoint& p_toPoint)
{
    // Simple case where 4 points will be needed
    m_Path.append(p_fromPoint);

    QPoint l_Point2;
    QPoint l_Point3;

    // Horizontal or vertical connection?
    eConnectDirection m_ConnectionFromDirection = m_ConnectFrom->getConnectDirectionFromPos(p_fromPoint);
    if( (eConnectionDirection_Left == m_ConnectionFromDirection)  || (eConnectionDirection_Right == m_ConnectionFromDirection) )
    {
        // Horizontal
        if( p_toPoint.y() != p_fromPoint.y() )
        {
            l_Point2.setX((p_fromPoint.x() + p_toPoint.x()) / 2);
            l_Point2.setY(p_fromPoint.y());
            m_Path.append(l_Point2);

            l_Point3.setX(l_Point2.x());
            l_Point3.setY(p_toPoint.y());
            m_Path.append(l_Point3);
        }
        else
        {
            // Nothing to do, it's a straight line
        }
    }
    else if( (eConnectionDirection_Top == m_ConnectionFromDirection)  || (eConnectionDirection_Bottom == m_ConnectionFromDirection)  )
    {
        // Vertical
        if( p_toPoint.x() != p_fromPoint.x() )
        {
            l_Point2.setX(p_fromPoint.x());
            l_Point2.setY((p_fromPoint.y() + p_toPoint.y()) / 2);
            m_Path.append(l_Point2);

            l_Point3.setX(p_toPoint.x());
            l_Point3.setY(l_Point2.y());
            m_Path.append(l_Point3);
        }
        else
        {
            // Nothing to do, it's a straight line
        }
    }
    else
    {
        // Not handled cases, direct link
    }

    m_Path.append(p_toPoint);
}

void Connector::routeBackToBack(const QPoint& p_fromPoint, const QPoint& p_toPoint)
{
    // 6 points needed
    QPoint l_Point2;
    QPoint l_Point3;
    QPoint l_Point4;
    QPoint l_Point5;

    const QRect l_fromCollisionRect = m_ConnectFrom->getCollisionRectangle();
    const QRect l_toCollisionRect = m_ConnectTo->getCollisionRectangle();

    m_Path.append(p_fromPoint);

    // Horizontal or vertical connection?
    eConnectDirection m_ConnectionFromDirection = m_ConnectFrom->getConnectDirectionFromPos(p_fromPoint);
    if( (eConnectionDirection_Left == m_ConnectionFromDirection)  || (eConnectionDirection_Right == m_ConnectionFromDirection) )
    {
        // Horizontal
        // Point 2 shall handle clearance from point 1
        if( eConnectionDirection_Left == m_ConnectionFromDirection )
        {
            l_Point2.setX(p_fromPoint.x() - this->getClearanceFromConnectables() );
            l_Point2.setY(p_fromPoint.y());

            l_Point5.setX(p_toPoint.x() + this->getClearanceFromConnectables() );
            l_Point5.setY(p_toPoint.y());
        }
        else // Right
        {
            l_Point2.setX(p_fromPoint.x() + this->getClearanceFromConnectables() );
            l_Point2.setY(p_fromPoint.y());

            l_Point5.setX(p_toPoint.x() - this->getClearanceFromConnectables() );
            l_Point5.setY(p_toPoint.y());
        }

        // For algo simplicity, always go around from top
        // (which is a rough simplification)
        int l_targetY;
        if( l_fromCollisionRect.y() < l_toCollisionRect.y() )
        {
            l_targetY = l_fromCollisionRect.y() - this->getClearanceFromConnectables();
        }
        else
        {
            l_targetY = l_toCollisionRect.y() - this->getClearanceFromConnectables();
        }

        l_Point3.setX(l_Point2.x());
        l_Point3.setY(l_targetY);

        l_Point4.setX(l_Point5.x());
        l_Point4.setY(l_targetY);
    }
    else // Bottom or Top (or at least considered as corners are less constraining)
    {
        // Vertical
        // Point 2 shall handle clearance from point 1
        if( eConnectionDirection_Bottom == m_ConnectionFromDirection )
        {
            l_Point2.setX(p_fromPoint.x());
            l_Point2.setY(p_fromPoint.y() + this->getClearanceFromConnectables() );

            l_Point5.setX(p_toPoint.x());
            l_Point5.setY(p_toPoint.y() - this->getClearanceFromConnectables() );
        }
        else // Top
        {
            l_Point2.setX(p_fromPoint.x());
            l_Point2.setY(p_fromPoint.y() - this->getClearanceFromConnectables() );

            l_Point5.setX(p_toPoint.x());
            l_Point5.setY(p_toPoint.y() + this->getClearanceFromConnectables() );
        }

        // For algo simplicity, always go around from left
        // (which is a rough simplification)
        int l_targetX;
        if( l_fromCollisionRect.x() < l_toCollisionRect.x() )
        {
            l_targetX = l_fromCollisionRect.x() - this->getClearanceFromConnectables();
        }
        else
        {
            l_targetX = l_toCollisionRect.x() - this->getClearanceFromConnectables();
        }

        l_Point3.setX(l_targetX);
        l_Point3.setY(l_Point2.y());

        l_Point4.setX(l_targetX);
        l_Point4.setY(l_Point5.y());
    }

    m_Path.append(l_Point2);
    m_Path.append(l_Point3);
    m_Path.append(l_Point4);
    m_Path.append(l_Point5);

    m_Path.append(p_toPoint);
}

void Connector::routeFaceToBack(const QPoint& p_fromPoint, const QPoint& p_toPoint)
{
    QPoint l_Point2;
    QPoint l_Point3;
    QPoint l_Point4;
    QPoint l_Point5;

    const QRect l_toCollisionRect = m_ConnectTo->getCollisionRectangle();

    m_Path.append(p_fromPoint);

    eConnectDirection m_ConnectionFromDirection = m_ConnectFrom->getConnectDirectionFromPos(p_fromPoint);

    switch (m_ConnectionFromDirection)
    {
    case eConnectionDirection_Left:
        if( (p_fromPoint.y() < l_toCollisionRect.top()) || (p_fromPoint.y() > l_toCollisionRect.bottom()) )
        {
            // No need to go all the way around
            l_Point2.setX(l_toCollisionRect.x() - this->getClearanceFromConnectables());
            l_Point2.setY(p_fromPoint.y());
            m_Path.append(l_Point2);
        }
        else
        {
            l_Point2.setX(p_fromPoint.x() - this->getClearanceFromConnectables());
            l_Point2.setY(p_fromPoint.y());
            m_Path.append(l_Point2);

            // Find shortest way (top or bottom)
            l_Point3.setX(l_Point2.x());
            if( abs(p_fromPoint.y() - l_toCollisionRect.top()) < abs(p_fromPoint.y() - l_toCollisionRect.bottom()) )
            {
                // Top is shorter
                l_Point3.setY(l_toCollisionRect.top() - this->getClearanceFromConnectables() );
            }
            else
            {
                l_Point3.setY(l_toCollisionRect.bottom() + this->getClearanceFromConnectables() );
            }
            m_Path.append(l_Point3);

            l_Point4.setX(p_toPoint.x() - this->getClearanceFromConnectables());
            l_Point4.setY(l_Point3.y());
            m_Path.append(l_Point4);
        }

        l_Point5.setX(p_toPoint.x() - this->getClearanceFromConnectables());
        l_Point5.setY(p_toPoint.y());
        m_Path.append(l_Point5);
        break;
    case eConnectionDirection_Right:
        if( (p_fromPoint.y() < l_toCollisionRect.top()) || (p_fromPoint.y() > l_toCollisionRect.bottom()) )
        {
            // No need to go all the way around
            l_Point2.setX(l_toCollisionRect.right() + this->getClearanceFromConnectables());
            l_Point2.setY(p_fromPoint.y());
            m_Path.append(l_Point2);
        }
        else
        {
            l_Point2.setX(p_fromPoint.x() + this->getClearanceFromConnectables());
            l_Point2.setY(p_fromPoint.y());
            m_Path.append(l_Point2);

            // Find shortest way (top or bottom)
            l_Point3.setX(l_Point2.x());
            if( abs(p_fromPoint.y() - l_toCollisionRect.top()) < abs(p_fromPoint.y() - l_toCollisionRect.bottom()) )
            {
                // Top is shorter
                l_Point3.setY(l_toCollisionRect.top() - this->getClearanceFromConnectables() );
            }
            else
            {
                l_Point3.setY(l_toCollisionRect.bottom() + this->getClearanceFromConnectables() );
            }
            m_Path.append(l_Point3);

            l_Point4.setX(p_toPoint.x() + this->getClearanceFromConnectables());
            l_Point4.setY(l_Point3.y());
            m_Path.append(l_Point4);
        }

        l_Point5.setX(p_toPoint.x() + this->getClearanceFromConnectables());
        l_Point5.setY(p_toPoint.y());
        m_Path.append(l_Point5);
        break;
    case eConnectionDirection_Top:
        if( (p_fromPoint.x() < l_toCollisionRect.left()) || (p_fromPoint.x() > l_toCollisionRect.right()) )
        {
            // No need to go all the way around
            l_Point2.setX(p_fromPoint.x());
            l_Point2.setY(l_toCollisionRect.top() - this->getClearanceFromConnectables());
            m_Path.append(l_Point2);
        }
        else
        {
            l_Point2.setX(p_fromPoint.x());
            l_Point2.setY(p_fromPoint.y() - this->getClearanceFromConnectables());
            m_Path.append(l_Point2);

            // Find shortest way (left or right)
            if( abs(p_fromPoint.x() - l_toCollisionRect.left()) < abs(p_fromPoint.x() - l_toCollisionRect.right()) )
            {
                // Left is shorter
                l_Point3.setX(l_toCollisionRect.left() - this->getClearanceFromConnectables());
            }
            else
            {
                l_Point3.setX(l_toCollisionRect.right() + this->getClearanceFromConnectables());
            }
            l_Point3.setY(l_Point2.y());
            m_Path.append(l_Point3);

            l_Point4.setX(l_Point3.x());
            l_Point4.setY(p_toPoint.y() - this->getClearanceFromConnectables());
            m_Path.append(l_Point4);
        }

        l_Point5.setX(p_toPoint.x());
        l_Point5.setY(p_toPoint.y() - this->getClearanceFromConnectables());
        m_Path.append(l_Point5);
        break;
    case eConnectionDirection_Bottom:
        if( (p_fromPoint.x() < l_toCollisionRect.left()) || (p_fromPoint.x() > l_toCollisionRect.right()) )
        {
            // No need to go all the way around
            l_Point2.setX(p_fromPoint.x());
            l_Point2.setY(l_toCollisionRect.bottom() + this->getClearanceFromConnectables());
            m_Path.append(l_Point2);
        }
        else
        {
            l_Point2.setX(p_fromPoint.x());
            l_Point2.setY(p_fromPoint.y() + this->getClearanceFromConnectables());
            m_Path.append(l_Point2);

            // Find shortest way (left or right)
            if( abs(p_fromPoint.x() - l_toCollisionRect.left()) < abs(p_fromPoint.x() - l_toCollisionRect.right()) )
            {
                // Left is shorter
                l_Point3.setX(l_toCollisionRect.left() - this->getClearanceFromConnectables());
            }
            else
            {
                l_Point3.setX(l_toCollisionRect.right() + this->getClearanceFromConnectables());
            }
            l_Point3.setY(l_Point2.y());
            m_Path.append(l_Point3);

            l_Point4.setX(l_Point3.x());
            l_Point4.setY(p_toPoint.y() + this->getClearanceFromConnectables());
            m_Path.append(l_Point4);
        }

        l_Point5.setX(p_toPoint.x());
        l_Point5.setY(p_toPoint.y() + this->getClearanceFromConnectables());
        m_Path.append(l_Point5);
        break;
    default:
        break;
    }

    m_Path.append(p_toPoint);
}

void Connector::routeBackToFace(const QPoint& p_fromPoint, const QPoint& p_toPoint)
{
    QPoint l_Point2;
    QPoint l_Point3;
    QPoint l_Point4;
    QPoint l_Point5;

    const QRect l_fromCollisionRect = m_ConnectFrom->getCollisionRectangle();

    m_Path.append(p_fromPoint);

    eConnectDirection m_ConnectionFromDirection = m_ConnectFrom->getConnectDirectionFromPos(p_fromPoint);

    switch (m_ConnectionFromDirection)
    {
    case eConnectionDirection_Left:
        l_Point2.setX(p_fromPoint.x() - this->getClearanceFromConnectables());
        l_Point2.setY(p_fromPoint.y());
        m_Path.append(l_Point2);

        l_Point3.setX(l_Point2.x());
        if( (p_toPoint.y() < l_fromCollisionRect.top()) || (p_toPoint.y() > l_fromCollisionRect.bottom()) )
        {
            if( (p_toPoint.x() - this->getClearanceFromConnectables()) < l_Point2.x() )
            {
                // In this case, the shortcut needs space with to target element
                l_Point2.setX(p_toPoint.x() - this->getClearanceFromConnectables());
                m_Path.last() = l_Point2;
            }
            // Means we can go straight at the last Y
            l_Point3.setY(p_toPoint.y());
            m_Path.append(l_Point3);
        }
        else if( abs(p_fromPoint.y() - l_fromCollisionRect.top()) < abs(p_fromPoint.y() - l_fromCollisionRect.bottom()) )
        {
            // Top seems to be the shortest
            l_Point3.setY(l_fromCollisionRect.top() - this->getClearanceFromConnectables());
            m_Path.append(l_Point3);

            l_Point4.setX(p_toPoint.x() - this->getClearanceFromConnectables());
            l_Point4.setY(l_Point3.y());
            m_Path.append(l_Point4);

            l_Point5.setX(l_Point4.x());
            l_Point5.setY(p_toPoint.y());
            m_Path.append(l_Point5);
        }
        else
        {
            l_Point3.setY(l_fromCollisionRect.bottom() + this->getClearanceFromConnectables());
            m_Path.append(l_Point3);

            l_Point4.setX(p_toPoint.x() - this->getClearanceFromConnectables());
            l_Point4.setY(l_Point3.y());
            m_Path.append(l_Point4);

            l_Point5.setX(l_Point4.x());
            l_Point5.setY(p_toPoint.y());
            m_Path.append(l_Point5);
        }

        break;
    case eConnectionDirection_Right:
        l_Point2.setX(p_fromPoint.x() + this->getClearanceFromConnectables());
        l_Point2.setY(p_fromPoint.y());
        m_Path.append(l_Point2);

        l_Point3.setX(l_Point2.x());
        if( (p_toPoint.y() < l_fromCollisionRect.top()) || (p_toPoint.y() > l_fromCollisionRect.bottom()) )
        {
            if( (p_toPoint.x() + this->getClearanceFromConnectables()) > l_Point2.x() )
            {
                // In this case, the shortcut needs space with to target element
                l_Point2.setX(p_toPoint.x() + this->getClearanceFromConnectables());
                m_Path.last() = l_Point2;
            }
            // Means we can go straight at the last Y
            l_Point3.setY(p_toPoint.y());
            m_Path.append(l_Point3);
        }
        else if( abs(p_fromPoint.y() - l_fromCollisionRect.top()) < abs(p_fromPoint.y() - l_fromCollisionRect.bottom()) )
        {
            // Top seems to be the shortest
            l_Point3.setY(l_fromCollisionRect.top() - this->getClearanceFromConnectables());
            m_Path.append(l_Point3);

            l_Point4.setX(p_toPoint.x() + this->getClearanceFromConnectables());
            l_Point4.setY(l_Point3.y());
            m_Path.append(l_Point4);

            l_Point5.setX(l_Point4.x());
            l_Point5.setY(p_toPoint.y());
            m_Path.append(l_Point5);
        }
        else
        {
            l_Point3.setY(l_fromCollisionRect.bottom() + this->getClearanceFromConnectables());
            m_Path.append(l_Point3);

            l_Point4.setX(p_toPoint.x() + this->getClearanceFromConnectables());
            l_Point4.setY(l_Point3.y());
            m_Path.append(l_Point4);

            l_Point5.setX(l_Point4.x());
            l_Point5.setY(p_toPoint.y());
            m_Path.append(l_Point5);
        }

        break;
    case eConnectionDirection_Top:
        l_Point2.setX(p_fromPoint.x());
        l_Point2.setY(p_fromPoint.y() - this->getClearanceFromConnectables());
        m_Path.append(l_Point2);

        if( (p_toPoint.x() < l_fromCollisionRect.left()) || (p_toPoint.x() > l_fromCollisionRect.right()) )
        {
            // Means we can go straight at the last Y
            if( (p_toPoint.y() - this->getClearanceFromConnectables()) < l_Point2.y() )
            {
                // In this case, the shortcut needs space with to target element
                l_Point2.setY(p_toPoint.y() - this->getClearanceFromConnectables());
                m_Path.last() = l_Point2;
            }
            // Means we can go straight at the last Y
            l_Point3.setX(p_toPoint.x());
            l_Point3.setY(l_Point2.y());
            m_Path.append(l_Point3);
        }
        else if( abs(p_fromPoint.y() - l_fromCollisionRect.left()) < abs(p_fromPoint.y() - l_fromCollisionRect.right()) )
        {
            // Left seems to be the shortest
            l_Point3.setX(l_fromCollisionRect.left() - this->getClearanceFromConnectables());
            l_Point3.setY(l_Point2.y());
            m_Path.append(l_Point3);

            l_Point4.setX(l_Point3.x());
            l_Point4.setY(p_toPoint.y() - this->getClearanceFromConnectables());
            m_Path.append(l_Point4);

            l_Point5.setX(p_toPoint.x());
            l_Point5.setY(l_Point4.y());
            m_Path.append(l_Point5);
        }
        else
        {
            l_Point3.setX(l_fromCollisionRect.right() + this->getClearanceFromConnectables());
            l_Point3.setY(l_Point2.y());
            m_Path.append(l_Point3);

            l_Point4.setX(l_Point3.x());
            l_Point4.setY(p_toPoint.y() - this->getClearanceFromConnectables());
            m_Path.append(l_Point4);

            l_Point5.setX(p_toPoint.x());
            l_Point5.setY(l_Point4.y());
            m_Path.append(l_Point5);
        }

        break;
    case eConnectionDirection_Bottom:
        l_Point2.setX(p_fromPoint.x());
        l_Point2.setY(p_fromPoint.y() + this->getClearanceFromConnectables());
        m_Path.append(l_Point2);

        if( (p_toPoint.x() < l_fromCollisionRect.left()) || (p_toPoint.x() > l_fromCollisionRect.right()) )
        {
            // Means we can go straight at the last Y
            if( (p_toPoint.y() + this->getClearanceFromConnectables()) > l_Point2.y() )
            {
                // In this case, the shortcut needs space with to target element
                l_Point2.setY(p_toPoint.y() + this->getClearanceFromConnectables());
                m_Path.last() = l_Point2;
            }
            l_Point3.setX(p_toPoint.x());
            l_Point3.setY(l_Point2.y());
            m_Path.append(l_Point3);
        }
        else if( abs(p_fromPoint.y() - l_fromCollisionRect.left()) < abs(p_fromPoint.y() - l_fromCollisionRect.right()) )
        {
            // Left seems to be the shortest
            l_Point3.setX(l_fromCollisionRect.left() - this->getClearanceFromConnectables());
            l_Point3.setY(l_Point2.y());
            m_Path.append(l_Point3);

            l_Point4.setX(l_Point3.x());
            l_Point4.setY(p_toPoint.y() + this->getClearanceFromConnectables());
            m_Path.append(l_Point4);

            l_Point5.setX(p_toPoint.x());
            l_Point5.setY(l_Point4.y());
            m_Path.append(l_Point5);
        }
        else
        {
            l_Point3.setX(l_fromCollisionRect.right() + this->getClearanceFromConnectables());
            l_Point3.setY(l_Point2.y());
            m_Path.append(l_Point3);

            l_Point4.setX(l_Point3.x());
            l_Point4.setY(p_toPoint.y() + this->getClearanceFromConnectables());
            m_Path.append(l_Point4);

            l_Point5.setX(p_toPoint.x());
            l_Point5.setY(l_Point4.y());
            m_Path.append(l_Point5);
        }

        break;
    default:
        break;
    }

    m_Path.append(p_toPoint);
}

void Connector::routeFaceToSide(const QPoint& p_fromPoint, const QPoint& p_toPoint)
{
    QPoint l_Point2;
    QPoint l_Point3;
    QPoint l_Point4;
    QPoint l_Point5;

    eConnectDirection m_ConnectionFromDirection = m_ConnectFrom->getConnectDirectionFromPos(p_fromPoint);
    eConnectDirection m_ConnectionToDirection = m_ConnectTo->getConnectDirectionFromPos(p_toPoint);

    m_Path.append(p_fromPoint);

    if( ((m_ConnectionToDirection == eConnectionDirection_Left) && (p_fromPoint.x() < (p_toPoint.x() - this->getClearanceFromConnectables()))) ||
            ((m_ConnectionToDirection == eConnectionDirection_Right) && (p_fromPoint.x() > (p_toPoint.x() + this->getClearanceFromConnectables()))) ||
            ((m_ConnectionToDirection == eConnectionDirection_Top) && (p_fromPoint.y() < (p_toPoint.y() - this->getClearanceFromConnectables()))) ||
            ((m_ConnectionToDirection == eConnectionDirection_Bottom) && (p_fromPoint.y() > (p_toPoint.y() + this->getClearanceFromConnectables()))) )
    {
        // Path is direct, only one intermediate
        if( (m_ConnectionFromDirection == eConnectionDirection_Left) ||
                (m_ConnectionFromDirection == eConnectionDirection_Right) )
        {
            // Create horizontal intermediate
            l_Point2.setX(p_toPoint.x());
            l_Point2.setY(p_fromPoint.y());
        }
        else
        {
            // Create vertical intermetiade
            l_Point2.setX(p_fromPoint.x());
            l_Point2.setY(p_toPoint.y());
        }
        m_Path.append(l_Point2);
    }
    else
    {
        switch(m_ConnectionFromDirection)
        {
        case eConnectionDirection_Left:
            l_Point2.setX(p_fromPoint.x() - this->getClearanceFromConnectables());
            l_Point2.setY(p_fromPoint.y());
            break;
        case eConnectionDirection_Right:
            l_Point2.setX(p_fromPoint.x() + this->getClearanceFromConnectables());
            l_Point2.setY(p_fromPoint.y());
            break;
        case eConnectionDirection_Top:
            l_Point2.setX(p_fromPoint.x());
            l_Point2.setY(p_fromPoint.y() - this->getClearanceFromConnectables());
            break;
        case eConnectionDirection_Bottom:
            l_Point2.setX(p_fromPoint.x());
            l_Point2.setY(p_fromPoint.y() + this->getClearanceFromConnectables());
            break;
        default:
            break;
        }

        switch(m_ConnectionToDirection)
        {
        case eConnectionDirection_Left:
            l_Point5.setX(p_toPoint.x() - this->getClearanceFromConnectables());
            l_Point5.setY(p_toPoint.y());
            break;
        case eConnectionDirection_Right:
            l_Point5.setX(p_toPoint.x() + this->getClearanceFromConnectables());
            l_Point5.setY(p_toPoint.y());
            break;
        case eConnectionDirection_Top:
            l_Point5.setX(p_toPoint.x());
            l_Point5.setY(p_toPoint.y() - this->getClearanceFromConnectables());
            break;
        case eConnectionDirection_Bottom:
            l_Point5.setX(p_toPoint.x());
            l_Point5.setY(p_toPoint.y() + this->getClearanceFromConnectables());
            break;
        default:
            break;
        }

        m_Path.append(l_Point2);
        if( (l_Point2.x() == l_Point5.x()) || (l_Point2.x() == l_Point5.x()) )
        {
            // No need for intermediate
            // So Point3 and 4 will not be used
        }
        else
        {
            // Need to know if connection is horizontal or vertical
            if( m_ConnectionFromDirection == eConnectionDirection_Left ||
                m_ConnectionFromDirection == eConnectionDirection_Right )
            {
                // Horizontal
                l_Point3.setX((p_fromPoint.x() + p_toPoint.x()) / 2);
                l_Point3.setY(l_Point2.y());
                m_Path.append(l_Point3);
                l_Point4.setX(l_Point3.x());
                l_Point4.setY(l_Point5.y());
                m_Path.append(l_Point4);
            }
            else
            {
                // Vertical
                l_Point3.setX(l_Point2.x());
                l_Point3.setY((p_fromPoint.y() + p_toPoint.y()) / 2);
                m_Path.append(l_Point3);
                l_Point4.setX(l_Point5.x());
                l_Point4.setY(l_Point3.y());
                m_Path.append(l_Point4);
            }
        }
        m_Path.append(l_Point5);
    }

    m_Path.append(p_toPoint);
}

void Connector::routeBackToSide(const QPoint& p_fromPoint, const QPoint& p_toPoint)
{
    QPoint l_Point2;
    QPoint l_Point3;
    QPoint l_Point4;
    QPoint l_Point5;
    QPoint l_Point6;

    const QRect l_fromCollisionRect = m_ConnectFrom->getCollisionRectangle();
    const QRect l_toCollisionRect = m_ConnectTo->getCollisionRectangle();

    m_Path.append(p_fromPoint);

    switch(m_ConnectionFromDirection)
    {
    case eConnectionDirection_Left:
        l_Point2.setX(p_fromPoint.x() - this->getClearanceFromConnectables());
        l_Point2.setY(p_fromPoint.y());
        m_Path.append(l_Point2);

        // Is top or bottom the shortest way?
        l_Point3.setX(l_Point2.x());
        if( p_toPoint.y() > p_fromPoint.y() )
        {
            // Bottom
            l_Point3.setY(l_fromCollisionRect.bottom() + this->getClearanceFromConnectables());
            m_Path.append(l_Point3);
        }
        else
        {
            // Top
            l_Point3.setY(l_fromCollisionRect.top() - this->getClearanceFromConnectables());
        }
        m_Path.append(l_Point3);

        // Check if end of path is direct
        if( (l_Point3.y() > (l_toCollisionRect.bottom() + this->getClearanceFromConnectables())) ||
                (l_Point3.y() < (l_toCollisionRect.top() - this->getClearanceFromConnectables())) )
        {
            l_Point4.setX(p_toPoint.x());
            l_Point4.setY(l_Point3.y());
            m_Path.append(l_Point4);
        }
        else
        {
            l_Point4.setX((p_fromPoint.x() + p_toPoint.x()) / 2);
            l_Point4.setY(l_Point3.y());
            m_Path.append(l_Point4);

            l_Point5.setX(l_Point4.x());
            if( m_ConnectionToDirection == eConnectionDirection_Bottom )
            {
                l_Point5.setY(p_toPoint.y() + this->getClearanceFromConnectables());
            }
            else
            {
                l_Point5.setY(p_toPoint.y() - this->getClearanceFromConnectables());
            }
            m_Path.append(l_Point5);

            l_Point6.setX(p_toPoint.x());
            l_Point6.setY(l_Point5.y());
            m_Path.append(l_Point6);
        }
        break;
    case eConnectionDirection_Right:
        l_Point2.setX(p_fromPoint.x() + this->getClearanceFromConnectables());
        l_Point2.setY(p_fromPoint.y());
        m_Path.append(l_Point2);

        // Is top or bottom the shortest way?
        l_Point3.setX(l_Point2.x());
        if( p_toPoint.y() > p_fromPoint.y() )
        {
            // Bottom
            l_Point3.setY(l_fromCollisionRect.bottom() + this->getClearanceFromConnectables());
            m_Path.append(l_Point3);
        }
        else
        {
            // Top
            l_Point3.setY(l_fromCollisionRect.top() - this->getClearanceFromConnectables());
        }
        m_Path.append(l_Point3);

        // Check if end of path is direct
        if( (l_Point3.y() > (l_toCollisionRect.bottom() + this->getClearanceFromConnectables())) ||
                (l_Point3.y() < (l_toCollisionRect.top() - this->getClearanceFromConnectables())) )
        {
            l_Point4.setX(p_toPoint.x());
            l_Point4.setY(l_Point3.y());
            m_Path.append(l_Point4);
        }
        else
        {
            l_Point4.setX((p_fromPoint.x() + p_toPoint.x()) / 2);
            l_Point4.setY(l_Point3.y());
            m_Path.append(l_Point4);

            l_Point5.setX(l_Point4.x());
            if( m_ConnectionToDirection == eConnectionDirection_Bottom )
            {
                l_Point5.setY(p_toPoint.y() + this->getClearanceFromConnectables());
            }
            else
            {
                l_Point5.setY(p_toPoint.y() - this->getClearanceFromConnectables());
            }
            m_Path.append(l_Point5);

            l_Point6.setX(p_toPoint.x());
            l_Point6.setY(l_Point5.y());
            m_Path.append(l_Point6);
        }
        break;
    case eConnectionDirection_Top:
        l_Point2.setX(p_fromPoint.x());
        l_Point2.setY(p_fromPoint.y() - this->getClearanceFromConnectables());
        m_Path.append(l_Point2);

        // Is top or bottom the shortest way?
        if( p_toPoint.x() > p_fromPoint.x() )
        {
            // Right
            l_Point3.setX(l_fromCollisionRect.right() + this->getClearanceFromConnectables());
        }
        else
        {
            // Left
            l_Point3.setX(l_fromCollisionRect.left() - this->getClearanceFromConnectables());
        }
        l_Point3.setY(l_Point2.y());
        m_Path.append(l_Point3);

        // Check if end of path is direct
        if( (l_Point3.x() > (l_toCollisionRect.right() + this->getClearanceFromConnectables())) ||
                (l_Point3.x() < (l_toCollisionRect.left() - this->getClearanceFromConnectables())) )
        {
            l_Point4.setX(l_Point3.x());
            l_Point4.setY(p_toPoint.y());
            m_Path.append(l_Point4);
        }
        else
        {
            l_Point4.setX(l_Point3.x());
            l_Point4.setY((p_fromPoint.y() + p_toPoint.y()) / 2);
            m_Path.append(l_Point4);

            if( m_ConnectionToDirection == eConnectionDirection_Left )
            {
                l_Point5.setX(p_toPoint.x() - this->getClearanceFromConnectables());
            }
            else
            {
                l_Point5.setX(p_toPoint.x() + this->getClearanceFromConnectables());
            }
            l_Point5.setY(l_Point4.y());
            m_Path.append(l_Point5);

            l_Point6.setX(l_Point5.x());
            l_Point6.setY(p_toPoint.y());
            m_Path.append(l_Point6);
        }
        break;
    case eConnectionDirection_Bottom:
        l_Point2.setX(p_fromPoint.x());
        l_Point2.setY(p_fromPoint.y() + this->getClearanceFromConnectables());
        m_Path.append(l_Point2);

        // Is top or bottom the shortest way?
        if( p_toPoint.x() > p_fromPoint.x() )
        {
            // Right
            l_Point3.setX(l_fromCollisionRect.right() + this->getClearanceFromConnectables());
        }
        else
        {
            // Left
            l_Point3.setX(l_fromCollisionRect.left() - this->getClearanceFromConnectables());
        }
        l_Point3.setY(l_Point2.y());
        m_Path.append(l_Point3);

        // Check if end of path is direct
        if( (l_Point3.x() > (l_toCollisionRect.right() + this->getClearanceFromConnectables())) ||
                (l_Point3.x() < (l_toCollisionRect.left() - this->getClearanceFromConnectables())) )
        {
            l_Point4.setX(l_Point3.x());
            l_Point4.setY(p_toPoint.y());
            m_Path.append(l_Point4);
        }
        else
        {
            l_Point4.setX(l_Point3.x());
            l_Point4.setY((p_fromPoint.y() + p_toPoint.y()) / 2);
            m_Path.append(l_Point4);

            if( m_ConnectionToDirection == eConnectionDirection_Left )
            {
                l_Point5.setX(p_toPoint.x() - this->getClearanceFromConnectables());
            }
            else
            {
                l_Point5.setX(p_toPoint.x() + this->getClearanceFromConnectables());
            }
            l_Point5.setY(l_Point4.y());
            m_Path.append(l_Point5);

            l_Point6.setX(l_Point5.x());
            l_Point6.setY(p_toPoint.y());
            m_Path.append(l_Point6);
        }
        break;
    default:
        break;
    }

    m_Path.append(p_toPoint);
}

void Connector::routeSideToFace(const QPoint& p_fromPoint, const QPoint& p_toPoint)
{
    const I_Connectable* l_tempConnectableTo = m_ConnectTo;
    const I_Connectable* l_tempConnectableFrom = m_ConnectFrom;

    // Invert to use already defined method
    m_ConnectFrom = l_tempConnectableTo;
    m_ConnectTo = l_tempConnectableFrom;

    this->routeFaceToSide(p_toPoint, p_fromPoint);

    m_ConnectFrom = l_tempConnectableFrom;
    m_ConnectTo = l_tempConnectableTo;

    this->invertPath();
}

void Connector::routeSideToBack(const QPoint& p_fromPoint, const QPoint& p_toPoint)
{
    // TODO: Link this method, it's not used yet
    const I_Connectable* l_tempConnectableTo = m_ConnectTo;
    const I_Connectable* l_tempConnectableFrom = m_ConnectFrom;

    // Invert to use already defined method
    m_ConnectFrom = l_tempConnectableTo;
    m_ConnectTo = l_tempConnectableFrom;

    this->routeBackToSide(p_toPoint, p_fromPoint);

    m_ConnectFrom = l_tempConnectableFrom;
    m_ConnectTo = l_tempConnectableTo;

    this->invertPath();
}

void Connector::routeSideToSideSelf(const QPoint& p_fromPoint, const QPoint& p_toPoint)
{
    m_Path.append(p_fromPoint);

    QPoint l_Point2;
    QPoint l_Point3;

    switch( m_ConnectFrom->getConnectDirectionFromPos(p_fromPoint) )
    {
    case eConnectionDirection_Left:
    {
        l_Point2.setX(p_fromPoint.x() - this->getClearanceFromConnectables());
        l_Point2.setY(p_fromPoint.y());
        l_Point3.setX(p_fromPoint.x() - this->getClearanceFromConnectables());
        l_Point3.setY(p_toPoint.y());
        break;
    }
    case eConnectionDirection_Right:
    {
        l_Point2.setX(p_fromPoint.x() + this->getClearanceFromConnectables());
        l_Point2.setY(p_fromPoint.y());
        l_Point3.setX(p_fromPoint.x() + this->getClearanceFromConnectables());
        l_Point3.setY(p_toPoint.y());
        break;
    }
    case eConnectionDirection_Top:
    {
        l_Point2.setX(p_fromPoint.x());
        l_Point2.setY(p_fromPoint.y() - this->getClearanceFromConnectables());
        l_Point3.setX(p_toPoint.x());
        l_Point3.setY(p_toPoint.y() - this->getClearanceFromConnectables());
        break;
    }
    case eConnectionDirection_Bottom:
    {
        l_Point2.setX(p_fromPoint.x());
        l_Point2.setY(p_fromPoint.y() + this->getClearanceFromConnectables());
        l_Point3.setX(p_toPoint.x());
        l_Point3.setY(p_toPoint.y() + this->getClearanceFromConnectables());
        break;
    }
    default:
        break;
    }

    m_Path.append(l_Point2);
    m_Path.append(l_Point3);
    m_Path.append(p_toPoint);
}

void Connector::invertPath()
{
    QList<QPoint> l_tempList;

    for(unsigned short i_point = 0U; i_point<m_Path.count(); i_point++ )
    {
        l_tempList.append(m_Path[m_Path.count() - 1U - i_point]);
    }

    m_Path.clear();
    m_Path = l_tempList;
}
