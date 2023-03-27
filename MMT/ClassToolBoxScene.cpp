#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

#include "ClassToolBoxScene.h"
#include "MouseToolPolygon.h"

#include "ConnectableTool.h"
#include "ContainerTool.h"
#include "ConnectorTool.h"

#include "MouseTool.h"
#include "ClassGraphicsItem.h"
#include "ExternalClassGraphicsItem.h"
#include "GeneralizationGraphicsItem.h"
#include "CompositionGraphicsItem.h"
#include "AggregationGraphicsItem.h"
#include "ImplementationGraphicsItem.h"

static const unsigned short s_heightPerTool = 80U;
static const unsigned short s_sceneWidth = 100U;
static const unsigned short s_border = 10U;
static const unsigned short s_nbTools = 7U;

ClassToolBoxScene::ClassToolBoxScene():
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

    // Class tool
    m_ToolsList.append(new ContainerTool<ClassGraphicsItem>());
    qreal w = 50;
    qreal h = 60;
    qreal dx = s_sceneWidth/2 - w/2;
    qreal dy = 2*s_heightPerTool - s_heightPerTool/2 - h/2;
    QGraphicsRectItem* l_ClassTool = this->addRect(QRectF(dx, dy, w, h));
    l_ClassTool->setBrush(QColor("blanchedalmond"));

    // External class tool
    m_ToolsList.append(new ContainerTool<ExternalClassGraphicsItem>());
    dy = 3*s_heightPerTool - s_heightPerTool/2 - h/2;
    QGraphicsRectItem* l_eClassTool = this->addRect(QRectF(dx, dy, w, h));
    l_eClassTool->setBrush(Qt::yellow);

    // Generalize tool
    m_ToolsList.append(new ConnectorTool<GeneralizationGraphicsItem>());
    qreal len = 70;
    qreal tipW = 20;
    qreal tipH = 40;
    dx = s_sceneWidth/2 - len/2;
    dy = 4*s_heightPerTool - s_heightPerTool/2;
    // The line
    this->addLine(QLineF(dx, dy, dx+len, dy));
    // The left arrow side
    this->addLine(QLine(dx + len, dy, dx + len - tipW, dy - tipH/2));
    this->addLine(QLine(dx + len, dy, dx + len - tipW, dy + tipH/2));

    // Compose tool
    m_ToolsList.append(new ConnectorTool<CompositionGraphicsItem>());
    len = 70;
    tipW = 20;
    tipH = 20;
    dx = s_sceneWidth/2 - len/2;
    dy = 5*s_heightPerTool - s_heightPerTool/2;
    // The line
    this->addLine(QLineF(dx, dy, dx+len-tipH, dy));
    // The tip
    QPolygonF l_polyCompose;
    l_polyCompose << QPointF(dx+len, 0 + dy)
           << QPointF(-tipH/2 + dx+len, tipW/2 + dy)
           << QPointF(-tipH + dx+len, dy)
           << QPointF(-tipH/2 + dx+len, dy - tipW/2);
    QGraphicsPolygonItem* l_ComposeTip = this->addPolygon(l_polyCompose);
    l_ComposeTip->setBrush(Qt::black);

    // Aggregate tool
    m_ToolsList.append(new ConnectorTool<AggregationGraphicsItem>());
    len = 70;
    tipW = 20;
    tipH = 20;
    dx = s_sceneWidth/2 - len/2;
    dy = 6*s_heightPerTool - s_heightPerTool/2;
    // The line
    this->addLine(QLineF(dx, dy, dx+len-tipH, dy));
    // The tip
    QPolygonF l_polyAgregate;
    l_polyAgregate << QPointF(dx+len, 0 + dy)
           << QPointF(-tipH/2 + dx+len, tipW/2 + dy)
           << QPointF(-tipH + dx+len, dy)
           << QPointF(-tipH/2 + dx+len, dy - tipW/2);
    this->addPolygon(l_polyAgregate);

    // Implement tool
    m_ToolsList.append(new ConnectorTool<ImplementationGraphicsItem>());
    len = 70;
    tipW = 20;
    tipH = 20;
    dx = s_sceneWidth/2 - len/2;
    dy = 7*s_heightPerTool - s_heightPerTool/2;
    // The line
    QGraphicsLineItem* l_ImplementLine = this->addLine(QLineF(dx, dy, dx+len-tipH, dy));
    QPen l_LinePen(Qt::PenStyle::DashLine);
    l_ImplementLine->setPen(l_LinePen);
    // The left arrow side
    this->addLine(QLine(dx + len, dy, dx + len - tipW, dy - tipH/2));
    this->addLine(QLine(dx + len, dy, dx + len - tipW, dy + tipH/2));

    ClassToolBoxScene::selectByID(0U);
}

void ClassToolBoxScene::mousePressEvent(QGraphicsSceneMouseEvent* p_Event)
{
    QPointF l_Pos = p_Event->scenePos();

    // X is not important here, only Y is.
    if( l_Pos.y() > (s_heightPerTool*s_nbTools) )
    {
        l_Pos.setY(s_heightPerTool*(s_nbTools-1U));
    }
    else if( l_Pos.y() < 0 )
    {
        l_Pos.setY(0);
    }
    l_Pos.setY(l_Pos.y() - qint16(l_Pos.y()) % s_heightPerTool);

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

unsigned short ClassToolBoxScene::getToolsQuantity() const
{
    return m_ToolsList.count();
}
const QList<I_Tool*>& ClassToolBoxScene::getToolsList() const
{
    return m_ToolsList;
}

void ClassToolBoxScene::selectByID(unsigned short p_ID)
{
    I_ToolBox::selectByID(p_ID);

    // Graphics
    if( nullptr != m_SelectionRect)
    {
        m_SelectionRect->setRect(QRect(10, this->getCurrentToolID()*s_heightPerTool, 80, 80));
    }
    else
    {
        m_SelectionRect = this->addRect(QRectF(10, this->getCurrentToolID()*s_heightPerTool, 80, 80));
    }

}
