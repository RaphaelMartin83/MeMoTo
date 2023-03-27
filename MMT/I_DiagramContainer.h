#ifndef I_DIAGRAMCONTAINER_H
#define I_DIAGRAMCONTAINER_H

#include <QGraphicsScene>
#include <QJsonObject>

#include "FindConfiguration.h"
#include "ConfigWidget.h"
#include "DiagramGraphicsView.h"

#include "I_ToolBox.h"
#include "I_ToolSwitcherListener.h"
#include "I_ConnectableContainer.h"
#include "I_Connector.h"
#include "I_ContainerGraphicsItem.h"
#include "I_ContainersContainer.h"

class I_DiagramContainer:
        public QGraphicsScene,
        public I_ToolSwitcherListener,
        public I_FindConfigurationListener
{
public:
    I_DiagramContainer();

    virtual I_ToolBox* getToolBox() = 0;
    virtual QString getDiagramString() = 0;
    virtual unsigned int getDiagramMaxWidth() const = 0;
    virtual unsigned int getDiagramMaxHeight() const = 0;
    virtual QPoint getStartPosition() const = 0;

    // Notification that something happened
    void changed(I_GraphicsItem* p_WhoChanged);

    // Access to Diagram content
    QList<I_Connector*> getAllConnectors() const;
    virtual QList<I_Selectable*> getAllSelectables(bool p_recursive=false) const;

    // User inputs and usability
    void deletePressed();
    void escapePressed();
    void printPressed();
    void selectToolByID(unsigned short p_ID);
    void mousePressEvent(QGraphicsSceneMouseEvent* p_Event);

    // View management
    void registerDiagramView(DiagramGraphicsView* p_View);
    DiagramGraphicsView* getView();
    void showDiagram();
    void setUnfocused();
    void setCurrentPosition(QPointF p_Position);

    // I_ToolSwitchListener
    void toolChanged();

    // I_ToolListener
    void addSelectable(I_Selectable* p_Selectable);
    void addConnectable(I_Connectable* p_Connectable);
    void addContainer(I_ContainersContainer* p_Container);
    void addConnector(GraphicConnector* p_Connector);
    void deleteConnectorAt(unsigned int p_ConnectableNumber);
    void deleteConnectableConnections(I_Connectable* p_Connectable);
    bool isConnectableContainerAtPos(QPoint p_Pos) const {return true;};

    // I_Serializable
    QJsonObject toJson();
    void fromJson(QJsonObject p_Json);

    /**
     * Find into diagram feature
    **/
    QStringList getDiagramTypes();
    void find();

    // FindCongigurationListener
    QStringList getFieldsOfType(QString p_Type);
    QStringList getAllDataOfTypeAndField(QString p_Type, QString p_Field);
    void focusOnItem(QString p_Type, QString p_Field,
                     QString p_Data, unsigned short p_findOffset,
                     bool p_Select = false);
    void findConfigurationValidated(QString p_Type,
                                    QString p_Field,
                                    QString p_Data,
                                    unsigned int p_selectionOffset);
    void findConfigurationCanceled();
    /**
     * End of find feature
    **/

    void clearAll();

private:
    void rerouteConnections();
    I_ContainerGraphicsItem* getContainerAtPos(QPoint p_Pos);

    // Data
    QList<I_Connector*> m_Connectors;

    // View control
    DiagramGraphicsView* m_View;
    bool m_isFocused;
    QPointF m_CurrentPosition;
    bool m_isFirstDisplay;
};

#endif // I_DIAGRAMCONTAINER_H
