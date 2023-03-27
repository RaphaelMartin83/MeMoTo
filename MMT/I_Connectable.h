#ifndef I_CONNECTABLE_H
#define I_CONNECTABLE_H

#include <QString>
#include <QPoint>
#include <QRect>
#include <QGraphicsItem>

#include "I_Selectable.h"

#include "ConnectionDefinitions.h"
#include "HandleGraphicsItem.h"

static const unsigned short CONNECTION_MAGNETIC_SELECTION_MIN_DIST = 50U;

class I_Connectable: public I_Selectable
{
public:

    virtual QString getConnectableType() const = 0;
    virtual const QList<HandleGraphicsItem*>& getConnectableHandles() const = 0;
    // Assuming every connectable is contained into a rectangle
    // A connector shall be able to wrap around the connectable
    virtual QString getConnectableName() const = 0;
    virtual bool isConnectable() const {return true;};
    virtual const I_Connectable* getConnectable(const QString& p_ConnectableType, const unsigned int& p_connectableID) const
    {
        if( (this->getConnectableType() == p_ConnectableType) &&
                (this->getID() == p_connectableID) )
        {
            return this;
        }
        else
        {
            return nullptr;
        }
    }

    virtual eConnectDirection getConnectDirectionFromPos(QPoint p_Pos) const
    {
        int l_handleID = -1;
        int l_dist = 65535;
        for( unsigned short i_handles=0U;
             (i_handles < this->getConnectableHandles().count());
             i_handles++ )
        {
            int l_tmpDist = std::sqrt(std::pow(p_Pos.x() - this->getConnectableHandles()[i_handles]->getHandleCoordinates().x(), 2) +
                    std::pow(p_Pos.y() - this->getConnectableHandles()[i_handles]->getHandleCoordinates().y(), 2));
            if( l_tmpDist < l_dist )
            {
                l_dist = l_tmpDist;
                if( l_dist < CONNECTION_MAGNETIC_SELECTION_MIN_DIST )
                {
                    l_handleID = i_handles;
                }
            }
        }

        eConnectDirection l_ret = eConnectionDirection_None;
        if( -1 != l_handleID )
        {
            l_ret = this->getConnectableHandles()[l_handleID]->getHandlePosition();
        }

        return l_ret;
    }

    virtual unsigned short getConnectionIDFromPos(QPoint p_Pos) const
    {
        // This method makes the hypothesis that (for comprehension)
        // All sub objects will create their handles from top to bottom and left to right
        int l_handleID = -1;
        eConnectDirection l_ConnecDirection = eConnectionDirection_None;
        int l_dist = 65535;

        for( unsigned short i_handles=0U;
             (i_handles < this->getConnectableHandles().count());
             i_handles++ )
        {
            int l_tmpDist = std::sqrt(std::pow(p_Pos.x() - this->getConnectableHandles()[i_handles]->getHandleCoordinates().x(), 2) +
                    std::pow(p_Pos.y() - this->getConnectableHandles()[i_handles]->getHandleCoordinates().y(), 2));
            if( l_tmpDist < l_dist )
            {
                l_dist = l_tmpDist;
                if( l_dist < CONNECTION_MAGNETIC_SELECTION_MIN_DIST )
                {
                    l_handleID = i_handles;
                }
            }
        }

        unsigned int l_ConnectorID = 0U;
        bool l_found = false;
        if( -1 != l_handleID )
        {
            l_ConnecDirection = this->getConnectableHandles()[l_handleID]->getHandlePosition();
            while( (false == l_found) && (l_handleID >= l_ConnectorID) )
            {
                if( this->getConnectableHandles()[l_handleID - l_ConnectorID]->getHandlePosition() != l_ConnecDirection )
                {
                    l_found = true;
                }
                else
                {
                    l_ConnectorID++;
                }
            }
            l_ConnectorID--;
        }
        else
        {
            l_ConnectorID = 0U;
        }

        return l_ConnectorID;
    }

    virtual QPoint getConnectionPosFromIDAndDirection(unsigned short p_ID, eConnectDirection p_Direction) const
    {
        // If the ID doesn't exist for this direction (resize), returns the max ID possible
        unsigned short l_IDCounter = 0U;
        bool l_found = false;
        QPoint l_Pos;

        for( unsigned short i_Handles = 0U;
             (i_Handles < this->getConnectableHandles().count()) && (false == l_found);
             i_Handles++)
        {
            if( p_Direction == this->getConnectableHandles()[i_Handles]->getHandlePosition() )
            {
                if( 0U == i_Handles)
                {
                    // Just in case we don't find it
                    l_Pos = this->getConnectableHandles()[i_Handles]->getHandleCoordinates();
                }

                if( l_IDCounter == p_ID )
                {
                    l_Pos = this->getConnectableHandles()[i_Handles]->getHandleCoordinates();
                    l_found = true;
                }
                else
                {
                    l_IDCounter++;
                }
            }
        }
        return l_Pos;
    }

    virtual bool wouldYouConnect(QPoint p_Pos) const
    {
        int l_dist = 65535; // Let's assume no handles will be that far

        // To be sure instead of downcasting
        // We never know
        HandleGraphicsItem* l_HandleCandidate = nullptr;
        for( unsigned short i_handles = 0U;
         (i_handles < this->getConnectableHandles().count());
         i_handles++ )
        {
            int l_tmpDist = std::sqrt(std::pow(p_Pos.x() - this->getConnectableHandles()[i_handles]->getHandleCoordinates().x(), 2) +
                    std::pow(p_Pos.y() - this->getConnectableHandles()[i_handles]->getHandleCoordinates().y(), 2));
            if( l_tmpDist < l_dist )
            {
                l_dist = l_tmpDist;
                if( l_dist < CONNECTION_MAGNETIC_SELECTION_MIN_DIST )
                {
                    l_HandleCandidate = this->getConnectableHandles()[i_handles];
                }
            }
        }

        return (nullptr != l_HandleCandidate);
    }
    virtual bool selectForConnection(QPoint p_Pos, eConnectionPoint p_ConnectPoint) const
    {
        bool l_ret = false;

        int l_dist = 65535; // Let's assume no handles will be that far

        // To be sure instead of downcasting
        // We never know
        HandleGraphicsItem* l_HandleCandidate = nullptr;
        for( unsigned short i_handles = 0U;
         (i_handles < this->getConnectableHandles().count());
         i_handles++ )
        {
            int l_tmpDist = std::sqrt(std::pow(p_Pos.x() - this->getConnectableHandles()[i_handles]->getHandleCoordinates().x(), 2) +
                    std::pow(p_Pos.y() - this->getConnectableHandles()[i_handles]->getHandleCoordinates().y(), 2));
            if( l_tmpDist < l_dist )
            {
                l_dist = l_tmpDist;
                if( l_dist < CONNECTION_MAGNETIC_SELECTION_MIN_DIST )
                {
                    l_HandleCandidate = this->getConnectableHandles()[i_handles];
                }
            }
        }

        if( (p_ConnectPoint == eConnectionPoint::eConnectionPoint_From) &&
                (nullptr != l_HandleCandidate) )
        {
            m_SelectedHandleForConnectionFrom = l_HandleCandidate;
            m_SelectedHandleForConnectionFrom->selectForConnection();
            l_ret = true;
        }
        else if ( nullptr != l_HandleCandidate )
        {
            m_SelectedHandleForConnectionTo = l_HandleCandidate;
            m_SelectedHandleForConnectionTo->selectForConnection();
            l_ret = true;
        }

        return l_ret;
    }
    virtual void unselectForConnection() const
    {
        if( nullptr != m_SelectedHandleForConnectionFrom )
        {
            // Uses the one from another interface but works for now
            m_SelectedHandleForConnectionFrom->unselect();
            m_SelectedHandleForConnectionFrom = nullptr;
        }
        if( nullptr != m_SelectedHandleForConnectionTo )
        {
            // Uses the one from another interface but works for now
            m_SelectedHandleForConnectionTo->unselect();
            m_SelectedHandleForConnectionTo = nullptr;
        }
    }
    virtual QPoint getSelectedConnectionPositionFrom() const
    {
        QPoint l_ret;

        if( nullptr != m_SelectedHandleForConnectionFrom )
        {
            l_ret = m_SelectedHandleForConnectionFrom->getHandleCoordinates();
        }
        else
        {
            // Woops
            l_ret.setX(0);
            l_ret.setY(0);
        }

        return l_ret;
    }
    virtual QPoint getSelectedConnectionPositionTo() const
    {
        QPoint l_ret;

        if( nullptr != m_SelectedHandleForConnectionTo )
        {
            l_ret = m_SelectedHandleForConnectionTo->getHandleCoordinates();
        }
        else
        {
            // Woops
            l_ret.setX(0);
            l_ret.setY(0);
        }

        return l_ret;
    }

private:
    mutable HandleGraphicsItem* m_SelectedHandleForConnectionFrom = nullptr;
    mutable HandleGraphicsItem* m_SelectedHandleForConnectionTo = nullptr;
};

#endif // I_CONNECTABLE_H
