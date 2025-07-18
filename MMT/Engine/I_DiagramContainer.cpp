#include <QApplication>
#include <QClipboard>
#include <QGraphicsSceneMouseEvent>
#include <QFile>
#include <QtSvg/QSvgGenerator>

#include <MeMoToApplication.h>
#include <MeMoToThemeDefinition.h>

#include <Engine/I_DiagramContainer.h>
#include <Engine/MeMoToLoader.h>

#include <Sharing/SharingManager.h>

static FindConfiguration* s_FindConfigurationContext = nullptr;

I_DiagramContainer::I_DiagramContainer():
      m_Connectors()
    , m_View(nullptr)
    , m_isFocused(false)
    , m_CurrentPosition()
    , m_isFirstDisplay(true)
    , m_Listener(nullptr)
{
    this->setBackgroundBrush(SCENES_BACKGROUND_COLOR);
}

QPoint I_DiagramContainer::getCurrentCursorPosition() const
{
    QPoint l_Ret;

    l_Ret.setX(static_cast<int>(m_CurrentCursorPosition.x()));
    l_Ret.setY(static_cast<int>(m_CurrentCursorPosition.y()));

    return GridReferential::getPointOnGrid(l_Ret);
}

void I_DiagramContainer::registerDiagramListener(I_DiagramListener* listener)
{
    Q_ASSERT(nullptr != listener);
    m_Listener = listener;
}

void I_DiagramContainer::undo()
{
    I_Undoable::undo();
    SharingManager::getInstance().pushModifications();
}
void I_DiagramContainer::redo()
{
    I_Undoable::redo();
    SharingManager::getInstance().pushModifications();
}

void I_DiagramContainer::changed(I_GraphicsItem* p_WhoChanged)
{
    this->saveUndoState();

    Q_ASSERT(nullptr != m_Listener);
    m_Listener->diagramChanged();
    SharingManager::getInstance().pushModifications();
}

QList<I_Connector*> I_DiagramContainer::getAllConnectors() const
{
    return m_Connectors;
}
QList<I_Selectable*> I_DiagramContainer::getAllSelectables(bool p_recursive) const
{
    QList<I_Selectable*> l_Ret = I_ContainersContainer::getAllSelectables(p_recursive);

    for( unsigned int i_connectors = 0U;
         i_connectors < m_Connectors.count(); i_connectors++ )
    {
        l_Ret.append(m_Connectors[i_connectors]);
    }

    return l_Ret;
}

void I_DiagramContainer::deletePressed()
{
    bool l_hasChanged = false;
    // If anything is selected, delete it
    const QList<I_Selectable*>& l_Selectables = this->getPureSelectables();
    for( unsigned int i_items=0u; i_items < l_Selectables.count(); i_items++ )
    {
        if( this->getSelectableAt(i_items)->isFullySelected() )
        {
            this->getSelectableAt(i_items)->unselect();
            this->deleteSelectableAt(i_items);
            l_hasChanged = true;
        }
    }

    const QList<I_Connectable*>& l_Connectables = this->getPureConnectables();
    for( unsigned int i_items=0u; i_items < l_Connectables.count(); i_items++ )
    {
        if( this->getConnectableAt(i_items)->isFullySelected() )
        {
            this->deleteConnectableConnections(this->getConnectableAt(i_items));
            this->getConnectableAt(i_items)->unselect();
            this->deleteConnectableAt(i_items);
            l_hasChanged = true;
        }
    }

    for( unsigned int i_items=0U; i_items < this->getPureContainers().count(); i_items++ )
    {
        I_Selectable* l_SelectedChild = this->getPureContainers()[i_items]->getChildSelected();
        if( static_cast<I_ContainerGraphicsItem*>(this->getPureContainers()[i_items])->isFullySelected() )
        {
            QList<I_Connectable*> l_RecursiveConnectables = this->getPureContainers()[i_items]->getAllConnectables(true);
            for( unsigned int i_connectables = 0U; i_connectables < l_RecursiveConnectables.count(); i_connectables++ )
            {
                this->deleteConnectableConnections(l_RecursiveConnectables[i_connectables]);
            }
            this->deleteConnectableConnections(
                        static_cast<I_ContainerGraphicsItem*>(this->getPureContainers()[i_items]));
            static_cast<I_ContainerGraphicsItem*>(this->getPureContainers()[i_items])->unselect();
            this->deleteContainerAt(i_items);
            l_hasChanged = true;
        }
        else if( nullptr != l_SelectedChild )
        {
            I_Connectable* l_ConnectableSelectedChild = dynamic_cast<I_Connectable*>(l_SelectedChild);
            if( nullptr != l_ConnectableSelectedChild )
            {
                this->deleteConnectableConnections(l_ConnectableSelectedChild);
            }
            this->getPureContainers()[i_items]->deleteSelectedChild();
            l_hasChanged = true;
        }
    }

    for( unsigned int i_items=0U; i_items < m_Connectors.count(); i_items++ )
    {
        if( m_Connectors[i_items]->isFullySelected() )
        {
            m_Connectors[i_items]->unselect();
            delete m_Connectors[i_items];
            m_Connectors.removeAt(i_items);
            l_hasChanged = true;
        }
    }

    if(l_hasChanged)
    {
        this->changed(nullptr);
    }
}
void I_DiagramContainer::escapePressed()
{
    this->unselectAll();

    if( nullptr != this->getFromConnectable() )
    {
        this->getFromConnectable()->unselectForConnection();
        this->setFromConnectable(nullptr);
    }
    ConfigWidget::close();
}
int I_DiagramContainer::printPressed(QString p_OutputFile)
{
    QFile l_FileOutput(p_OutputFile);
    bool l_isFileWritable = l_FileOutput.open(QIODevice::WriteOnly);
    l_FileOutput.close();
    if( ("" != p_OutputFile) && (!l_isFileWritable) )
    {
        // Do not go default and avoid poluting user machine
        qDebug() << "Output file could not be written";
        return -1;
    }

    // Get a bigger ROI
    QRectF l_ROI = this->itemsBoundingRect();

    this->clearSelection();
    this->setSceneRect(l_ROI);

    QSvgGenerator generator;
    generator.setFileName(("" != p_OutputFile) ? l_FileOutput.fileName() : (this->getDiagramString() + ".svg"));
    generator.setSize(this->sceneRect().size().toSize());
    generator.setViewBox(QRect(0, 0,
                               this->sceneRect().size().width(),
                               this->sceneRect().size().height()));
    generator.setDescription(QObject::tr("MeMoTo diagram exported to SVG"));
    generator.setTitle(generator.fileName());
    QPainter painter;
    painter.begin(&generator);
    this->render(&painter);
    painter.end();

    this->setSceneRect(QRect(0, 0,
                             this->getDiagramMaxWidth(),
                             this->getDiagramMaxHeight()));

    return 0;
}
void I_DiagramContainer::copyPressed()
{
    QJsonObject l_MyJson;
    QList<I_Selectable*> l_CurrentSelectedItems = this->getCurrentSelected();
    QPoint l_CurrentSelectionCoord = this->getSelectionCoord();

    for( unsigned int i_con = 0U; i_con < l_CurrentSelectedItems.count(); i_con++ )
    {
        I_Selectable* l_CurrentSelectable = l_CurrentSelectedItems[i_con];
        if( nullptr != l_CurrentSelectable && l_CurrentSelectable->isFullySelected() )
        {
            QJsonObject::iterator l_ArrayFound = l_MyJson.find(l_CurrentSelectable->getSerializableName());
            QJsonArray l_Array;
            if( l_MyJson.end() != l_ArrayFound )
            {
                l_Array = l_ArrayFound->toArray();
            }
            l_Array.append(l_CurrentSelectable->toJson());
            l_MyJson.insert(l_CurrentSelectable->getSerializableName(), l_Array);
        }
    }

    // Encapsulate current diagram Json with diagram type name
    // This way, avoid mixing diagrams on paste
    QJsonObject l_JsonToCopy;
    l_JsonToCopy.insert(this->getDiagramString(), l_MyJson);
    l_JsonToCopy.insert("SelectionCoordX", l_CurrentSelectionCoord.x());
    l_JsonToCopy.insert("SelectionCoordY", l_CurrentSelectionCoord.y());

    QByteArray l_JsonText;
    MeMoToLoader::JsonToArray(l_JsonToCopy, l_JsonText);

    QClipboard* l_pClipboard = QApplication::clipboard();
    l_pClipboard->clear(QClipboard::Clipboard);
    l_pClipboard->setText(l_JsonText, QClipboard::Clipboard);
}
void I_DiagramContainer::pastePressed()
{
    QClipboard* l_pClipboard = QApplication::clipboard();
    QByteArray l_currentClipboard = l_pClipboard->text().toUtf8();

    QJsonObject l_JsonObject = MeMoToLoader::loadFromArray(l_currentClipboard);

    QPoint l_MiddlePointToPaste;
    QJsonObject::iterator l_tmpObject;
    l_tmpObject = l_JsonObject.find("SelectionCoordX");
    if( l_JsonObject.end() != l_tmpObject )
    {
        l_MiddlePointToPaste.setX(l_tmpObject->toInt());
    }
    else
    {
        // No chance this will be pasted safely
        return;
    }

    l_tmpObject = l_JsonObject.find("SelectionCoordY");
    if( l_JsonObject.end() != l_tmpObject )
    {
        l_MiddlePointToPaste.setY(l_tmpObject->toInt());
    }
    else
    {
        return;
    }

    l_tmpObject = l_JsonObject.find(this->getDiagramString());
    if( l_JsonObject.end() != l_tmpObject )
    {
        l_JsonObject = l_tmpObject->toObject();
    }
    else
    {
        // Trying to paste something not compliant for sure
        return;
    }

    this->pasteJson(l_JsonObject, l_MiddlePointToPaste);

    this->clearIDOverrides();
}
void I_DiagramContainer::selectToolByID(unsigned short p_ID)
{
    this->getToolBox()->selectByID(p_ID);
}
void I_DiagramContainer::selectNextTool()
{
    unsigned short l_ID = this->getToolBox()->getCurrentToolID();
    l_ID++;
    if( l_ID >= this->getToolBox()->getToolsList().count() )
    {
        l_ID = 0U;
    }
    this->selectToolByID(l_ID);
}
void I_DiagramContainer::selectPreviousTool()
{
    unsigned short l_ID = this->getToolBox()->getCurrentToolID();
    l_ID--;
    if( l_ID >= this->getToolBox()->getToolsList().count() )
    {
        l_ID = this->getToolBox()->getToolsList().count() - 1U;
    }
    this->selectToolByID(l_ID);
}
void I_DiagramContainer::mousePressEvent(QGraphicsSceneMouseEvent* p_Event)
{
    QPoint l_PosInt;
    l_PosInt.setX(p_Event->scenePos().x());
    l_PosInt.setY(p_Event->scenePos().y());

    switch(p_Event->buttons())
    {
        case Qt::MouseButton::LeftButton:
        {
            this->getToolBox()->useTool(l_PosInt, Qt::MouseButton::LeftButton);
            break;
        }
        case Qt::MouseButton::RightButton:
        {
            this->getToolBox()->useTool(l_PosInt, Qt::MouseButton::RightButton);
            break;
        }
        default:
        {
            // Unhandled
        }
    }
}
void I_DiagramContainer::mouseMoveEvent(QGraphicsSceneMouseEvent* p_Event)
{
    m_CurrentCursorPosition = p_Event->scenePos();
}

void I_DiagramContainer::registerDiagramView(DiagramGraphicsView* p_View)
{
    m_View = p_View;
}

DiagramGraphicsView* I_DiagramContainer::getView()
{
    return m_View;
}

void I_DiagramContainer::showDiagram()
{
    m_isFocused = true;
    m_View->setScene(this);
    if( true == m_isFirstDisplay )
    {
        m_View->centerOn(this->getStartPosition());
    }
    else
    {
        m_View->centerOn(m_CurrentPosition);
    }
    m_View->update();
    m_isFirstDisplay = false;
}

void I_DiagramContainer::setUnfocused()
{
    m_isFocused = false;
    m_CurrentPosition = m_View->mapToScene(m_View->viewport()->rect().center());
}

void I_DiagramContainer::setCurrentPosition(QPointF p_Position)
{
    if( true == m_isFocused )
    {
        m_View->centerOn(p_Position);
    }
    m_CurrentPosition = p_Position;
}

void I_DiagramContainer::toolChanged()
{
    this->unselectAll();
    if( nullptr != this->getFromConnectable() )
    {
        this->getFromConnectable()->unselectForConnection();
        this->setFromConnectable(nullptr);
    }
}
void I_DiagramContainer::addSelectable(I_Selectable* p_Selectable)
{
    I_ContainersContainer::addSelectable(p_Selectable);

    this->addItem(p_Selectable);
    this->rerouteConnections();
}
void I_DiagramContainer::addConnectable(I_Connectable* p_Connectable)
{
    I_ContainersContainer::addConnectable(p_Connectable);

    this->addItem(p_Connectable);
    this->rerouteConnections();
}
void I_DiagramContainer::addContainer(I_ContainersContainer* p_Container)
{
    I_ContainersContainer::addContainer(p_Container);

    this->addItem(dynamic_cast<QGraphicsItem*>(p_Container));
    this->rerouteConnections();
}
void I_DiagramContainer::addConnector(GraphicConnector* p_Connector)
{
    this->addItem(p_Connector);
    m_Connectors.append(p_Connector);
}
void I_DiagramContainer::deleteConnectorAt(unsigned int p_ConnectableNumber)
{
    delete m_Connectors.takeAt(p_ConnectableNumber);
}
void I_DiagramContainer::deleteConnectableConnections(I_Connectable* p_Connectable)
{
    // Check if connectable is also a container
    I_ContainersContainer* l_Container = dynamic_cast<I_ContainersContainer*>(p_Connectable);

    for(unsigned int i_connectors = 0U;
        i_connectors < m_Connectors.count();
        i_connectors++ )
    {
        if( (p_Connectable == m_Connectors[i_connectors]->getConnectFrom()) ||
            (p_Connectable == m_Connectors[i_connectors]->getConnectTo()) )
        {
            this->deleteConnectorAt(i_connectors);
            i_connectors--;
        }
        else if( nullptr != l_Container )
        {
            if( l_Container->isItYourChild(m_Connectors[i_connectors]->getConnectFrom()) ||
                    l_Container->isItYourChild(m_Connectors[i_connectors]->getConnectTo()) )
            {
                this->deleteConnectorAt(i_connectors);
            }
        }
    }
}

QJsonObject I_DiagramContainer::toJson() const
{
    QJsonObject l_MyJson = I_ContainersContainer::toJson();

    for( unsigned int i_con = 0U; i_con < m_Connectors.count(); i_con++ )
    {
        I_Connector* l_CurrentSelectable = m_Connectors[i_con];
        if( nullptr != l_CurrentSelectable )
        {
            QJsonObject::iterator l_ArrayFound = l_MyJson.find(l_CurrentSelectable->getSerializableName());
            QJsonArray l_Array;
            if( l_MyJson.end() != l_ArrayFound )
            {
                l_Array = l_ArrayFound->toArray();
            }
            l_Array.append(l_CurrentSelectable->toJson());
            l_MyJson.insert(l_CurrentSelectable->getSerializableName(), l_Array);
        }
    }

    return l_MyJson;
}
void I_DiagramContainer::fromJson(const QJsonObject& p_Json)
{
    QList<unsigned short> l_ConnectorsIDs;

    for(unsigned short i_tools = 0U; i_tools < this->getToolBox()->getToolsQuantity(); i_tools++ )
    {
        if(this->getToolBox()->getToolsList()[i_tools]->createsConnectors())
        {
            // Connectors shall be created last
            l_ConnectorsIDs.append(i_tools);
        }
        else
        {
            QJsonObject::const_iterator l_ArrayFound = p_Json.find(this->getToolBox()->getToolsList()[i_tools]->getItemName());
            QJsonArray l_ItemTypeJson;
            if( p_Json.end() != l_ArrayFound )
            {
                l_ItemTypeJson = l_ArrayFound->toArray();
            }

            for(QJsonArray::const_iterator l_CurrentTypeIt = l_ItemTypeJson.begin();
                l_CurrentTypeIt < l_ItemTypeJson.end(); l_CurrentTypeIt++)
            {
                this->getToolBox()->getToolsList()[i_tools]->use(l_CurrentTypeIt->toObject(), this);

                // Containers may contain diagrams
                if(this->getToolBox()->getToolsList()[i_tools]->createsContainers())
                {
                    QJsonObject l_ContainerObject = l_CurrentTypeIt->toObject();
                    QJsonObject::const_iterator l_ContainedJson = l_ContainerObject.find("Contained");
                    if( l_ContainerObject.end() != l_ContainedJson )
                    {
                        this->fromJson(l_ContainedJson->toObject());
                    }
                }
            }
        }
    }

    for(unsigned short i_connectors = 0U; i_connectors < l_ConnectorsIDs.count(); i_connectors++)
    {
        QJsonObject::const_iterator l_ArrayFound = p_Json.find(this->getToolBox()->getToolsList()[l_ConnectorsIDs[i_connectors]]->getItemName());
        QJsonArray l_ItemTypeJson;
        if( p_Json.end() != l_ArrayFound )
        {
            l_ItemTypeJson = l_ArrayFound->toArray();
        }

        for(QJsonArray::const_iterator l_CurrentTypeIt = l_ItemTypeJson.begin();
        l_CurrentTypeIt < l_ItemTypeJson.end(); l_CurrentTypeIt++)
        {
            this->getToolBox()->getToolsList()[l_ConnectorsIDs[i_connectors]]->use(l_CurrentTypeIt->toObject(), this);
        }
    }
}

QStringList I_DiagramContainer::getDiagramTypes()
{
    QStringList l_Ret;

    QList<I_Selectable*> l_Selectables = this->getAllSelectables(true);
    for(unsigned int i_selectables = 0U; i_selectables < l_Selectables.count(); i_selectables++ )
    {
        if( !l_Ret.contains(l_Selectables[i_selectables]->getSelectableType()) )
        {
            l_Ret.append(l_Selectables[i_selectables]->getSelectableType());
        }
    }

    return l_Ret;
}

void I_DiagramContainer::find()
{
    if( nullptr == s_FindConfigurationContext )
    {
        s_FindConfigurationContext = new FindConfiguration();
    }

    // Get all diagram data
    QJsonObject l_JsonData = this->toJson();
    if( !l_JsonData.isEmpty() )
    {
        // Parse diagram data into types
        s_FindConfigurationContext->registerConfigListener(this);
        s_FindConfigurationContext->setSelectableTypes(this->getDiagramTypes());
        s_FindConfigurationContext->updateOccurencesList();

        ConfigWidget::open(s_FindConfigurationContext);
    }
}

QStringList I_DiagramContainer::getFieldsOfType(QString p_Type)
{
    QStringList l_Ret;

    QList<I_Selectable*> l_Selectables = this->getAllSelectables(true);
    bool l_found = false;
    for( unsigned int i_selectables = 0U;
         (i_selectables < l_Selectables.count()) && (false == l_found);
         i_selectables++ )
    {
        if( p_Type == l_Selectables[i_selectables]->getSelectableType() )
        {
            l_found = true;
            l_Ret = l_Selectables[i_selectables]->getSearchFields();
        }
    }

    return l_Ret;
}

QStringList I_DiagramContainer::getAllDataOfTypeAndField(QString p_Type, QString p_Field)
{
    QStringList l_Ret;

    QList<I_Selectable*> l_Selectables = this->getAllSelectables(true);
    for( unsigned int i_selectables = 0U;
         i_selectables < l_Selectables.count(); i_selectables++ )
    {
        if( (p_Type == l_Selectables[i_selectables]->getSelectableType()) &&
            ("" != l_Selectables[i_selectables]->getDataFromField(p_Field)) )
        {
            l_Ret.append(l_Selectables[i_selectables]->getDataFromField(p_Field));
        }
    }

    return l_Ret;
}

void I_DiagramContainer::focusOnItem(QString p_Type, QString p_Field,
         QString p_Data, unsigned short p_findOffset,
         bool p_Select, bool p_CaseSensitive)
{
    QList<I_Selectable*> l_Selectables = this->getAllSelectables(true);

    bool l_found = false;

    // Put this out of loop for optimization
    // If type is not specified
    if( ("" == p_Type) || ("" == p_Field) )
    {
        for( unsigned int i_selectables = 0U;
             (i_selectables < l_Selectables.count()) && (false == l_found);
             i_selectables++ )
        {
            QStringList l_Fields = l_Selectables[i_selectables]->getSearchFields();

            for( unsigned int i_fields = 0U; i_fields < l_Fields.count(); i_fields++ )
            {
                QString l_DataToFind = p_Data;
                QString l_CurrentData = l_Selectables[i_selectables]->getDataFromField(l_Fields[i_fields]);
                if( false == p_CaseSensitive )
                {
                    l_DataToFind = l_DataToFind.toLower();
                    l_CurrentData = l_CurrentData.toLower();
                }
                if( l_CurrentData == l_DataToFind )
                {
                    this->setCurrentPosition(l_Selectables[i_selectables]->getFocusPosition());
                    l_found = true;
                }
            }
        }
    }

    // This loop is strict search, all fields are filled
    // Case sensitivity is true
    for( unsigned int i_selectables = 0U;
         (i_selectables < l_Selectables.count()) && (false == l_found);
         i_selectables++ )
    {
        if( (p_Type == l_Selectables[i_selectables]->getSelectableType()) &&
            (p_Data == l_Selectables[i_selectables]->getDataFromField(p_Field)) )
        {
            if( 0U != p_findOffset )
            {
                p_findOffset--;
            }
            else
            {
                // Focus on selectable position
                l_found = true;

                if( true == m_isFocused )
                {
                    if( true == p_Select )
                    {
                        this->unselectAll();
                        l_Selectables[i_selectables]->select();
                    }
                }
                this->setCurrentPosition(l_Selectables[i_selectables]->getFocusPosition());
            }
        }
    }
}

void I_DiagramContainer::findConfigurationValidated(QString p_Type,
                QString p_Field,
                QString p_Data,
                unsigned int p_selectionOffset)
{
    // Close before opening selection
    ConfigWidget::close();

    this->focusOnItem(p_Type, p_Field, p_Data, p_selectionOffset, true);
}

void I_DiagramContainer::findConfigurationCanceled()
{
    ConfigWidget::close();
}

void I_DiagramContainer::clearAll()
{
    I_ContainersContainer::clearAll();

    while(0U != m_Connectors.count())
    {
        delete m_Connectors.takeAt(0U);
    }
}

void I_DiagramContainer::rerouteConnections()
{
    for( unsigned int i_connectors = 0U; i_connectors < m_Connectors.count(); i_connectors++ )
    {
        m_Connectors[i_connectors]->reroute();
    }
}

I_ContainerGraphicsItem* I_DiagramContainer::getContainerAtPos(QPoint p_Pos)
{
    I_ContainerGraphicsItem* l_Ret = nullptr;

    for( unsigned int i_tl = 0U;
         (i_tl < this->getPureContainers().count()) && (nullptr == l_Ret);
         i_tl++ )
    {
        if( static_cast<I_ContainerGraphicsItem*>(this->getPureContainers()[i_tl])->isItYou(p_Pos) )
        {
            l_Ret = static_cast<I_ContainerGraphicsItem*>(getPureContainers()[i_tl]);
        }
    }
    return l_Ret;
}

void I_DiagramContainer::pasteJson(const QJsonObject& p_Json, const QPoint& p_MiddlePointToPaste)
{
    QList<unsigned short> l_ConnectorsIDs;

    for(unsigned short i_tools = 0U; i_tools < this->getToolBox()->getToolsQuantity(); i_tools++ )
    {
        if(this->getToolBox()->getToolsList()[i_tools]->createsConnectors())
        {
            // Connectors shall be created last
            l_ConnectorsIDs.append(i_tools);
        }
        else
        {
            QJsonObject::const_iterator l_ArrayFound = p_Json.find(this->getToolBox()->getToolsList()[i_tools]->getItemName());
            QJsonArray l_ItemTypeJson;
            if( p_Json.end() != l_ArrayFound )
            {
                l_ItemTypeJson = l_ArrayFound->toArray();
            }

            for(QJsonArray::const_iterator l_CurrentTypeIt = l_ItemTypeJson.begin();
                l_CurrentTypeIt < l_ItemTypeJson.end(); l_CurrentTypeIt++)
            {
                QUuid l_RetrievedID;
                QUuid l_NewID;
                this->getToolBox()->getToolsList()[i_tools]->paste(l_CurrentTypeIt->toObject(),
                                                                   p_MiddlePointToPaste, this,
                                                                   &l_RetrievedID, &l_NewID);
                this->addIDOverride(l_RetrievedID, l_NewID);

                // Containers may contain diagrams
                if(this->getToolBox()->getToolsList()[i_tools]->createsContainers())
                {
                    QJsonObject l_ContainerObject = l_CurrentTypeIt->toObject();
                    QJsonObject::const_iterator l_ContainedJson = l_ContainerObject.find("Contained");
                    if( l_ContainerObject.end() != l_ContainedJson )
                    {
                        this->pasteJson(l_ContainedJson->toObject(), p_MiddlePointToPaste);
                    }
                }
            }
        }
    }

    for(unsigned short i_connectors = 0U; i_connectors < l_ConnectorsIDs.count(); i_connectors++)
    {
        QJsonObject::const_iterator l_ArrayFound = p_Json.find(this->getToolBox()->getToolsList()[l_ConnectorsIDs[i_connectors]]->getItemName());
        QJsonArray l_ItemTypeJson;
        if( p_Json.end() != l_ArrayFound )
        {
            l_ItemTypeJson = l_ArrayFound->toArray();
        }

        for(QJsonArray::iterator l_CurrentTypeIt = l_ItemTypeJson.begin();
            l_CurrentTypeIt < l_ItemTypeJson.end(); l_CurrentTypeIt++)
        {
            this->getToolBox()->getToolsList()[l_ConnectorsIDs[i_connectors]]->paste(l_CurrentTypeIt->toObject(),
                                                                                     p_MiddlePointToPaste, this);
        }
    }
}
