#include <MeMoToThemeDefinition.h>

#include <Engine/StackingDefinitions.h>
#include <Engine/I_DiagramContainer.h>

#include <GraphicsItems/StateGraphicsItem.h>
#include <CommonGraphics/ConfigWidget.h>

static const char* STATE_CONNECTABLE_NAME = "State";
static const unsigned short STATE_TEXT_LABELS_HEIGHT = 15U;
const char* StateGraphicsItem::SERIALIZABLE_NAME = "States";

StateGraphicsItem::StateGraphicsItem(const QPoint& p_Pos,
                                     unsigned short p_Width,
                                     unsigned short p_Height):
    I_SquarishGraphicsItem(p_Pos, QString("State"), p_Width, p_Height)
  , m_EntryAction("")
  , m_ExitAction("")
  , m_Stereotype("")
{
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
    m_EntryActionGraphicsItem->setDefaultTextColor(TEXT_GRAPHICS_COLOR);
    m_ExitActionGraphicsItem->setPos(this->getPos().x() - this->getWidth()/2,
                                     this->getPos().y() - this->getHeight()/2 + (STATE_TEXT_LABELS_HEIGHT*3));
    m_ExitActionGraphicsItem->setDefaultTextColor(TEXT_GRAPHICS_COLOR);
    m_StereotypeGraphicsItem->setPos(this->getPos().x() - this->getWidth()/2,
                                     this->getPos().y() - this->getHeight()/2 + (STATE_TEXT_LABELS_HEIGHT*4));
    m_StereotypeGraphicsItem->setDefaultTextColor(TEXT_GRAPHICS_COLOR);

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
    this->getConfig<StateConfiguration>().registerDiagram(this->getDiagramContainer());
    this->getConfig<StateConfiguration>().setListener(this->getSelectableType(), this->getID());

    // initializes module with parameters
    this->getConfig<StateConfiguration>().setName(this->getName());
    this->getConfig<StateConfiguration>().setEntryAction(m_EntryAction);
    this->getConfig<StateConfiguration>().setExitAction(m_ExitAction);
    this->getConfig<StateConfiguration>().setStereotype(m_Stereotype);
    this->getConfig<StateConfiguration>().setColor(this->getColorName());

    // Let's rock
    ConfigWidget::open(&this->getConfig<StateConfiguration>());
}
void StateGraphicsItem::applyConfiguration()
{
    this->setName(this->getConfig<StateConfiguration>().getName());
    this->setEntryAction(this->getConfig<StateConfiguration>().getEntryAction());
    this->setExitAction(this->getConfig<StateConfiguration>().getExitAction());
    this->setStereotype(this->getConfig<StateConfiguration>().getStereotype());
    this->setColor(this->getConfig<StateConfiguration>().getColor());

    this->getDiagramContainer()->changed(this);

    this->closeConfiguration();
}

QJsonObject StateGraphicsItem::toJson() const
{
    QJsonObject l_MyJson = I_SquarishGraphicsItem::toJson();

    l_MyJson.insert("EntryAction", m_EntryAction);
    l_MyJson.insert("ExitAction", m_ExitAction);
    l_MyJson.insert("Stereotype", m_Stereotype);

    return l_MyJson;
}

void StateGraphicsItem::fromJson(const QJsonObject& p_Json)
{
    I_SquarishGraphicsItem::fromJson(p_Json);

    this->setEntryAction(p_Json.find("EntryAction")->toString());
    this->setExitAction(p_Json.find("ExitAction")->toString());
    this->setStereotype(p_Json.find("Stereotype")->toString());

    this->refreshDisplay();
}

QString StateGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}
