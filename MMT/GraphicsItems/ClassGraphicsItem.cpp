#include <Engine/StackingDefinitions.h>
#include <Engine/I_DiagramContainer.h>

#include <ConfigurationContexts/ClassConfiguration.h>

#include <CommonGraphics/ConfigWidget.h>

#include "ClassGraphicsItem.h"

static const char* CLASS_CONNECTABLE_NAME = "Class";
const char* ClassGraphicsItem::SERIALIZABLE_NAME = "Classes";

ClassGraphicsItem::ClassGraphicsItem(const QPoint& p_Pos,
                            unsigned short p_Width,
                            unsigned short p_Height):
    I_ClassGraphicsItem(p_Pos, QString("Class"), p_Width, p_Height)
{
    ClassGraphicsItem::refreshDisplay();
}
ClassGraphicsItem::ClassGraphicsItem(const QJsonObject& p_JSon):
    I_ClassGraphicsItem(p_JSon)
{
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
    this->getConfig<ClassConfiguration>().registerDiagram(this->getDiagramContainer());
    this->getConfig<ClassConfiguration>().setListener(this->getSelectableType(), this->getID());

    // initializes module with parameters
    this->getConfig<ClassConfiguration>().setName(this->getName());
    this->getConfig<ClassConfiguration>().setMethods(this->getMethodsList());
    this->getConfig<ClassConfiguration>().setAttributes(this->getAttributesList());
    this->getConfig<ClassConfiguration>().setColor(this->getColorName());
    this->getConfig<ClassConfiguration>().setContentToHide(this->isContentToHide());

    // Let's rock
    ConfigWidget::open(&this->getConfig<ClassConfiguration>());
}
void ClassGraphicsItem::applyConfiguration()
{
    this->setContentToHide(this->getConfig<ClassConfiguration>().isContentToHide());
    this->setName(this->getConfig<ClassConfiguration>().getName());
    this->setMethodsList(this->getConfig<ClassConfiguration>().getMethods());
    this->setAttributesList(this->getConfig<ClassConfiguration>().getAttributes());
    this->setColor(this->getConfig<ClassConfiguration>().getColor());

    this->getDiagramContainer()->changed(this);

    this->closeConfiguration();
}

QString ClassGraphicsItem::getConnectableType() const
{
    return CLASS_CONNECTABLE_NAME;
}

QString ClassGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}
