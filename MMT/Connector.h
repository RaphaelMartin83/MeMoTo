#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QList>
#include <QPoint>

#include "I_Connectable.h"
#include "I_ConnectableContainer.h"
#include "I_Connector.h"

class Connector: public I_Connector
{
public:
    Connector(const I_Connectable* p_ConnectFrom,
              const I_Connectable* p_ConnectTo,
              const QPoint& p_fromPoint,
              const QPoint& p_toPoint,
              I_ConnectableContainer* p_Container);
    Connector(const I_Connectable* p_ConnectFrom,
              const I_Connectable* p_ConnectTo,
              const QPoint& p_fromPoint,
              const QPoint& p_toPoint,
              I_ConnectableContainer* p_Container,
              const QList<QPoint>& p_ForcedPath);
    Connector(I_Connectable* p_ConnectFrom,
              I_Connectable* p_ConnectTo,
              const QList<QPoint>& p_Path);
    Connector(const QJsonObject& p_JsonObject,
              I_ConnectableContainer* p_Container);

    virtual void reroute();

    const QList<QPoint>& getPath() const;
    void setPath(unsigned short p_index, QPoint p_Point);
    void setPath(const QList<QPoint>& p_Points);
    I_ConnectableContainer* getContainer();
    const I_Connectable* const getConnectFrom() const;
    void setConnectFrom(const I_Connectable* p_ConnectFrom);
    const I_Connectable* const getConnectTo() const;
    void setConnectTo(const I_Connectable* p_ConnectTo);
    eConnectDirection getConnectionFromDirection() const;
    void setConnectionFromDirection(eConnectDirection p_Direction);
    eConnectDirection getConnectionToDirection() const;
    void setConnectionToDirection(eConnectDirection p_Direction);
    unsigned short getConnectionFromID() const;
    void setConnectionFromID(unsigned short p_ID);
    unsigned short getConnectionToID() const;
    void setConnectionToID(unsigned short p_ID);
    bool getPathIsForced() const;
    void setPathIsForced(bool p_isForced);

    bool needReroute() const;
    void route(const QPoint& p_fromPoint, const QPoint& p_toPoint);

    // I_Serializable
    QJsonObject toJson();
    void fromJson(QJsonObject p_Json);

private:
    // Route algorithm is divided into multiple simple cases
    void routeFaceToFace(const QPoint& p_fromPoint, const QPoint& p_toPoint);
    void routeBackToFace(const QPoint& p_fromPoint, const QPoint& p_toPoint);
    void routeFaceToBack(const QPoint& p_fromPoint, const QPoint& p_toPoint);
    void routeBackToBack(const QPoint& p_fromPoint, const QPoint& p_toPoint);
    void routeFaceToSide(const QPoint& p_fromPoint, const QPoint& p_toPoint);
    void routeBackToSide(const QPoint& p_fromPoint, const QPoint& p_toPoint);
    void routeSideToFace(const QPoint& p_fromPoint, const QPoint& p_toPoint);
    void routeSideToBack(const QPoint& p_fromPoint, const QPoint& p_toPoint);
    void routeSideToSideSelf(const QPoint& p_fromPoint, const QPoint& p_toPoint);

    void invertPath();

    const I_Connectable* m_ConnectFrom;
    const I_Connectable* m_ConnectTo;
    QList<QPoint> m_Path;
    bool m_PathIsForced;

    // The direction
    eConnectDirection m_ConnectionFromDirection;
    eConnectDirection m_ConnectionToDirection;

    // The ID of the handle in the given direction
    // (from left to right or from top to bottom, corners have ID 0)
    unsigned short m_ConnectionFromID;
    unsigned short m_ConnectionToID;

    I_ConnectableContainer* m_Container;
};

#endif // CONNECTOR_H
