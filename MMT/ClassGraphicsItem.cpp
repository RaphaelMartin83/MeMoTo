#include "ClassGraphicsItem.h"
#include "ClassConfiguration.h"
#include "ConfigWidget.h"
#include "StackingDefinitions.h"
#include "I_DiagramContainer.h"

static const char* CLASS_CONNECTABLE_NAME = "Class";
const char* ClassGraphicsItem::SERIALIZABLE_NAME = "Classes";

static ClassConfiguration* s_ConfigurationContext;

ClassGraphicsItem::ClassGraphicsItem(const QPoint& p_Pos,
                            unsigned short p_Width,
                            unsigned short p_Height):
    I_ClassGraphicsItem(p_Pos, QString("Class"), p_Width, p_Height)
{
    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new ClassConfiguration();
    }

    ClassGraphicsItem::refreshDisplay();
}
ClassGraphicsItem::ClassGraphicsItem(const QJsonObject& p_JSon):
    I_ClassGraphicsItem(p_JSon)
{
    // Instanciate configuration layout if needed
    static bool ls_isConfigInited = false;
    if( false == ls_isConfigInited )
    {
        ls_isConfigInited = true;
        s_ConfigurationContext = new ClassConfiguration();
    }

    ClassGraphicsItem::refreshDisplay();
}

ClassGraphicsItem::~ClassGraphicsItem()
{

}

void ClassGraphicsItem::select()
{
    I_SquarishGraphicsItem::select();

    if( this->isFullySelected() )
    {
        this->openConfiguration();
    }
}
void ClassGraphicsItem::select(QPoint p_Pos)
{
    I_SquarishGraphicsItem::select(p_Pos);

    if( this->isFullySelected() )
    {
        this->openConfiguration();
    }
}

void ClassGraphicsItem::unselect()
{
    I_SquarishGraphicsItem::unselect();

    this->closeConfiguration();
}

QString ClassGraphicsItem::getSelectableType() const
{
    return SERIALIZABLE_NAME;
}

void ClassGraphicsItem::openConfiguration()
{
    s_ConfigurationContext->registerDiagram(this->getDiagramContainer());
    s_ConfigurationContext->setListener(this->getSelectableType(), this->getID());

    // initializes module with parameters
    s_ConfigurationContext->setName(this->getName());
    s_ConfigurationContext->setMethods(this->getMethodsList());
    s_ConfigurationContext->setAttributes(this->getAttributesList());
    s_ConfigurationContext->setColor(this->getColorName());
    s_ConfigurationContext->setContentToHide(this->isContentToHide());

    // Let's rock
    ConfigWidget::open(s_ConfigurationContext);
}
void ClassGraphicsItem::closeConfiguration()
{
    ConfigWidget::close();
}
void ClassGraphicsItem::applyConfiguration()
{
    this->getDiagramContainer()->changed(this);
    this->setContentToHide(s_ConfigurationContext->isContentToHide());
    this->setName(s_ConfigurationContext->getName());
    this->setMethodsList(s_ConfigurationContext->getMethods());
    this->setAttributesList(s_ConfigurationContext->getAttributes());
    this->setColor(s_ConfigurationContext->getColor());

    this->getDiagramContainer()->changed(this);

    ConfigWidget::close();
}

QString ClassGraphicsItem::getConnectableType() const
{
    return CLASS_CONNECTABLE_NAME;
}

QString ClassGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}
