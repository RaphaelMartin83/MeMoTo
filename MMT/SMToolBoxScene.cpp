#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

#include "SMToolBoxScene.h"
#include "MouseToolPolygon.h"

#include "ConnectableTool.h"
#include "ContainerTool.h"
#include "ConnectorTool.h"

#include "MouseTool.h"
#include "StateGraphicsItem.h"
#include "ChoiceGraphicsItem.h"
#include "TransitionGraphicsItem.h"
#include "EntryStateGraphicsItem.h"
#include "InitialStateGraphicsItem.h"

static const unsigned short s_heightPerTool = 100U;
static const unsigned short s_sceneWidth = 100U;
static const unsigned short s_border = 10U;
static const unsigned short s_nbTools = 6U;

SMToolBoxScene::SMToolBoxScene():
    m_SelectionRect(nullptr)
  , m_ToolsList()
{
    this->setSceneRect(QRectF(
                           s_border, s_border,
                           s_sceneWidth - 2*s_border, s_heightPerTool*s_nbTools - 2*s_border));

    // Mouse tool
    m_ToolsList.append(new MouseTool());
    qreal size = 30;
    MouseToolPolygon* l_MouseTool = new MouseToolPolygon(size, s_sceneWidth, s_heightPerTool);
    this->addPolygon(*l_MouseTool);

    // State tool
    m_ToolsList.append(new ContainerTool<StateGraphicsItem>());
    qreal w = 70;
    qreal h = 50;
    qreal dx = s_sceneWidth/2 - w/2;
    qreal dy = 2*s_heightPerTool - s_heightPerTool/2 - h/2;
    m_StateItem = this->addRect(QRectF(dx, dy, w, h));
    m_StateItem->setBrush(QColor("blanchedalmond"));

    // Choice tool
    m_ToolsList.append(new ConnectableTool<ChoiceGraphicsItem>());
    w = 50;
    h = 70;
    dx = s_sceneWidth/2 - w/2;
    dy = 3*s_heightPerTool - s_heightPerTool/2 - h/2;
    QPolygonF l_poly;
    l_poly << QPointF(w/2 + dx, 0 + dy) << QPointF(0 + dx, h/2 + dy) << QPointF(w/2 + dx, h + dy) << QPointF(w + dx, h/2 + dy);
    m_choiceItem = this->addPolygon(l_poly);
    m_choiceItem->setBrush(Qt::green);

    // Transition tool
    m_ToolsList.append(new ConnectorTool<TransitionGraphicsItem>());
    qreal len = 70;
    qreal tipW = 20;
    qreal tipH = 40;
    dx = s_sceneWidth/2 - len/2;
    dy = 4*s_heightPerTool - s_heightPerTool/2;
    // The line
    m_transitionItemLine = this->addLine(QLineF(dx, dy, dx+len, dy));
    // The left arrow side
    m_transitionItemArrowLeft = this->addLine(QLine(dx + len, dy, dx + len - tipW, dy - tipH/2));
    m_transitionItemArrowRight = this->addLine(QLine(dx + len, dy, dx + len - tipW, dy + tipH/2));

    // Init tool
    m_ToolsList.append(new ConnectableTool<InitialStateGraphicsItem>());
    qreal diameter = 40;
    dx = s_sceneWidth/2 - diameter/2;
    dy = 5*s_heightPerTool - s_heightPerTool/2 - diameter/2;
    m_InitItem = this->addEllipse(QRect(dx, dy, diameter, diameter));
    m_InitItem->setBrush(Qt::blue);

    // Entry tool
    m_ToolsList.append(new ConnectableTool<EntryStateGraphicsItem>());
    diameter = 40;
    dx = s_sceneWidth/2 - diameter/2;
    dy = 6*s_heightPerTool - s_heightPerTool/2 - diameter/2;
    m_EntryPointItem = this->addEllipse(QRect(dx, dy, diameter, diameter));
    m_EntryPointItem->setBrush(Qt::green);

    SMToolBoxScene::selectByID(0U);
}

void SMToolBoxScene::mousePressEvent(QGraphicsSceneMouseEvent* p_Event)
{
    QPointF l_Pos = p_Event->scenePos();

    // X is not important here, only Y is.
    if( l_Pos.y() > 600 )
    {
        l_Pos.setY(500);
    }
    else if( l_Pos.y() < 0 )
    {
        l_Pos.setY(0);
    }
    l_Pos.setY(l_Pos.y() - qint16(l_Pos.y()) % 100);

    if( nullptr != m_SelectionRect)
    {
        m_SelectionRect->setRect(QRect(10, l_Pos.y() + 10, 80, 80));
    }
    else
    {
        m_SelectionRect = this->addRect(QRectF(10, l_Pos.y() + 10, 80, 80));
    }

    this->selectByID(static_cast<unsigned short>(
                static_cast<unsigned int>(l_Pos.y()) / s_heightPerTool));
}

unsigned short SMToolBoxScene::getToolsQuantity() const
{
    return m_ToolsList.count();
}
const QList<I_Tool*>& SMToolBoxScene::getToolsList() const
{
    return m_ToolsList;
}

void SMToolBoxScene::selectByID(unsigned short p_ID)
{
    I_ToolBox::selectByID(p_ID);

    // Graphics
    if( nullptr != m_SelectionRect)
    {
        m_SelectionRect->setRect(QRect(10, this->getCurrentToolID()*s_heightPerTool + 10, 80, 80));
    }
    else
    {
        m_SelectionRect = this->addRect(QRectF(10, this->getCurrentToolID()*s_heightPerTool + 10, 80, 80));
    }

}
