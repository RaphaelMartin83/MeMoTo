#include "StateGraphicsItem.h"
#include "ConfigWidget.h"
#include "StackingDefinitions.h"
#include "I_DiagramContainer.h"

static const char* STATE_CONNECTABLE_NAME = "State";
static const unsigned short STATE_TEXT_LABELS_HEIGHT = 15U;
const char* StateGraphicsItem::SERIALIZABLE_NAME = "States";

static StateConfiguration* s_ConfigurationContext = nullptr;

// Enough to create one state per second until the sun explodes, and even more than that...
// Should be enough
static unsigned long long s_StatesUniqueIDs = 0U;

StateGraphicsItem::StateGraphicsItem(QPointF p_Pos,
                                     unsigned short p_Width,
                                     unsigned short p_Height):
    I_SquarishGraphicsItem(p_Pos, QString("State" + QString::number(s_StatesUniqueIDs)), s_StatesUniqueIDs, p_Width, p_Height)
  , m_EntryAction("")
  , m_ExitAction("")
  , m_Stereotype("")
{
    s_StatesUniqueIDs++;

    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new StateConfiguration();
    }

    m_EntryActionGraphicsItem = new QGraphicsTextItem(m_EntryAction);
    this->addToGroup(m_EntryActionGraphicsItem);

    m_ExitActionGraphicsItem = new QGraphicsTextItem(m_ExitAction);
    this->addToGroup(m_ExitActionGraphicsItem);

    m_StereotypeGraphicsItem = new QGraphicsTextItem(m_Stereotype);
    this->addToGroup(m_StereotypeGraphicsItem);

    StateGraphicsItem::refreshDisplay();
}

StateGraphicsItem::StateGraphicsItem(const QJsonObject& p_JSonObject):
    I_SquarishGraphicsItem(p_JSonObject)
{
    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new StateConfiguration();
    }

    if( this->getID() >= s_StatesUniqueIDs )
    {
        s_StatesUniqueIDs = this->getID() + 1U;
    }

    m_EntryActionGraphicsItem = new QGraphicsTextItem(m_EntryAction);
    this->addToGroup(m_EntryActionGraphicsItem);

    m_ExitActionGraphicsItem = new QGraphicsTextItem(m_ExitAction);
    this->addToGroup(m_ExitActionGraphicsItem);

    m_StereotypeGraphicsItem = new QGraphicsTextItem(m_Stereotype);
    this->addToGroup(m_StereotypeGraphicsItem);

    // Content
    this->setEntryAction(p_JSonObject.find("EntryAction")->toString());
    this->setExitAction(p_JSonObject.find("ExitAction")->toString());
    this->setStereotype(p_JSonObject.find("Stereotype")->toString());

    StateGraphicsItem::refreshDisplay();
}

StateGraphicsItem::~StateGraphicsItem()
{
    this->removeFromGroup(m_EntryActionGraphicsItem);
    delete m_EntryActionGraphicsItem;

    this->removeFromGroup(m_ExitActionGraphicsItem);
    delete m_ExitActionGraphicsItem;

    this->removeFromGroup(m_StereotypeGraphicsItem);
    delete m_StereotypeGraphicsItem;
}

void StateGraphicsItem::setEntryAction(const QString& p_EntryAction)
{
    m_EntryAction = p_EntryAction;
    if("" != m_EntryAction)
    {
        m_EntryActionGraphicsItem->setPlainText("-> " + m_EntryAction);
    }
    else
    {
        m_EntryActionGraphicsItem->setPlainText(m_EntryAction);
    }
}
const QString& StateGraphicsItem::getEntryAction() const
{
    return m_EntryAction;
}

void StateGraphicsItem::setExitAction(const QString& p_ExitAction)
{
    m_ExitAction = p_ExitAction;
    if("" != m_ExitAction)
    {
        m_ExitActionGraphicsItem->setPlainText("<- " + m_ExitAction);
    }
    else
    {
        m_ExitActionGraphicsItem->setPlainText(m_ExitAction);
    }
}
const QString& StateGraphicsItem::getExitAction() const
{
    return m_ExitAction;
}

void StateGraphicsItem::setStereotype(const QString& p_Stereotype)
{
    m_Stereotype = p_Stereotype;
    m_StereotypeGraphicsItem->setPlainText(m_Stereotype);
}
const QString& StateGraphicsItem::getStereotype() const
{
    return m_Stereotype;
}

void StateGraphicsItem::refreshDisplay()
{
    I_SquarishGraphicsItem::refreshDisplay();

    m_EntryActionGraphicsItem->setPos(this->getPos().x() - this->getWidth()/2,
                                      this->getPos().y() - this->getHeight()/2 + STATE_TEXT_LABELS_HEIGHT*2);
    m_ExitActionGraphicsItem->setPos(this->getPos().x() - this->getWidth()/2,
                                     this->getPos().y() - this->getHeight()/2 + (STATE_TEXT_LABELS_HEIGHT*3));
    m_StereotypeGraphicsItem->setPos(this->getPos().x() - this->getWidth()/2,
                                     this->getPos().y() - this->getHeight()/2 + (STATE_TEXT_LABELS_HEIGHT*4));

    this->setZValue(getDepthFromPlan(this->getPlan()));
}

QString StateGraphicsItem::getConnectableType() const
{
    return STATE_CONNECTABLE_NAME;
}

void StateGraphicsItem::select()
{
    I_SquarishGraphicsItem::select();

    if( this->isFullySelected() )
    {
        this->openConfiguration();
    }
}
void StateGraphicsItem::select(QPoint p_Pos)
{
    I_SquarishGraphicsItem::select(p_Pos);

    if( this->isFullySelected() )
    {
        this->openConfiguration();
    }
}

void StateGraphicsItem::unselect()
{
    I_SquarishGraphicsItem::unselect();

    this->closeConfiguration();
}

QString StateGraphicsItem::getSelectableType() const
{
    return SERIALIZABLE_NAME;
}
QStringList StateGraphicsItem::getSearchFields() const
{
    QStringList l_Ret;

    l_Ret.append("Name");
    l_Ret.append("EntryAction");
    l_Ret.append("ExitAction");
    l_Ret.append("Stereotype");

    return l_Ret;
}
QString StateGraphicsItem::getDataFromField(const QString& p_FieldName) const
{
    QString l_Ret;

    if( "Name" == p_FieldName )
    {
        l_Ret = this->getName();
    }
    else if( "EntryAction" == p_FieldName )
    {
        l_Ret = this->getEntryAction();
    }
    else if( "ExitAction" == p_FieldName )
    {
        l_Ret = this->getExitAction();
    }
    else if( "Setereotype" == p_FieldName )
    {
        l_Ret = this->getStereotype();
    }

    return l_Ret;
}

void StateGraphicsItem::openConfiguration()
{
    s_ConfigurationContext->registerDiagram(this->getDiagramContainer());
    s_ConfigurationContext->setListener(this->getSelectableType(), this->getID());

    // initializes module with parameters
    s_ConfigurationContext->setName(this->getName());
    s_ConfigurationContext->setEntryAction(m_EntryAction);
    s_ConfigurationContext->setExitAction(m_ExitAction);
    s_ConfigurationContext->setStereotype(m_Stereotype);
    s_ConfigurationContext->setColor(this->getColorName());

    // Let's rock
    ConfigWidget::open(s_ConfigurationContext);
}
void StateGraphicsItem::closeConfiguration()
{
    ConfigWidget::close();
}
void StateGraphicsItem::applyConfiguration()
{
    this->setName(s_ConfigurationContext->getName());
    this->setEntryAction(s_ConfigurationContext->getEntryAction());
    this->setExitAction(s_ConfigurationContext->getExitAction());
    this->setStereotype(s_ConfigurationContext->getStereotype());
    this->setColor(s_ConfigurationContext->getColor());

    this->getDiagramContainer()->changed(this);

    ConfigWidget::close();
}

QJsonObject StateGraphicsItem::toJson()
{
    QJsonObject l_MyJson = I_SquarishGraphicsItem::toJson();

    l_MyJson.insert("EntryAction", m_EntryAction);
    l_MyJson.insert("ExitAction", m_ExitAction);
    l_MyJson.insert("Stereotype", m_Stereotype);

    return l_MyJson;
}

void StateGraphicsItem::fromJson(QJsonObject p_Json)
{
}

QString StateGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}
