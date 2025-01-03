#include <QPen>

#include <Engine/I_DiagramContainer.h>

#include <CommonGraphics/ConfigWidget.h>
#include <GraphicsItems/ImplementationGraphicsItem.h>
#include <ConfigurationContexts/ImplementationConfiguration.h>

const char* ImplementationGraphicsItem::SERIALIZABLE_NAME="Implementations";

static int implementationStartReduction = 2;

ImplementationGraphicsItem::ImplementationGraphicsItem(
        const I_Connectable *p_ConnectFrom,
        const I_Connectable *p_ConnectTo,
        const QPoint &p_fromPoint,
        const QPoint &p_toPoint,
        I_ConnectableContainer* p_Container):
    GraphicConnector(p_ConnectFrom, p_ConnectTo, p_fromPoint, p_toPoint, p_Container)
  , m_Arrow()
{
    this->setStartReduction(implementationStartReduction);
    ImplementationGraphicsItem::refreshDisplay();
}

ImplementationGraphicsItem::ImplementationGraphicsItem(
        const I_Connectable *p_ConnectFrom,
        const I_Connectable *p_ConnectTo,
        const QPoint &p_fromPoint,
        const QPoint &p_toPoint,
        I_ConnectableContainer* p_Container,
        const QList<QPoint>& p_ForcedPath):
    GraphicConnector(p_ConnectFrom, p_ConnectTo, p_fromPoint, p_toPoint, p_Container, p_ForcedPath)
  , m_Arrow()
{
    this->setStartReduction(implementationStartReduction);
    ImplementationGraphicsItem::refreshDisplay();
}

ImplementationGraphicsItem::ImplementationGraphicsItem(const QJsonObject& p_JsonObject,
                                               I_ConnectableContainer* p_Container):
    GraphicConnector(p_JsonObject, p_Container)
  , m_Arrow()
{
    this->setStartReduction(implementationStartReduction);
    ImplementationGraphicsItem::refreshDisplay();
}

ImplementationGraphicsItem::~ImplementationGraphicsItem()
{

}

void ImplementationGraphicsItem::reroute()
{
    GraphicConnector::reroute();

    this->refreshDisplay();
}

QString ImplementationGraphicsItem::getSerializableName() const
{
    return SERIALIZABLE_NAME;
}

// I_Selectable overloads
void ImplementationGraphicsItem::select()
{
    GraphicConnector::select();

    if( true == this->isFullySelected() )
    {
        this->openConfiguration();
    }
}
void ImplementationGraphicsItem::select(QPoint p_Pos)
{
    GraphicConnector::select(p_Pos);

    if( true == this->isFullySelected() )
    {
        this->openConfiguration();
    }
}
void ImplementationGraphicsItem::unselect()
{
    GraphicConnector::unselect();

    this->closeConfiguration();
}
QString ImplementationGraphicsItem::getSelectableType() const
{
    return SERIALIZABLE_NAME;
}
QStringList ImplementationGraphicsItem::getSearchFields() const
{
    return QStringList("ID");
}
QString ImplementationGraphicsItem::getDataFromField(const QString& p_FieldName) const
{
    QString l_Ret;

    if( "ID" == p_FieldName )
    {
        l_Ret = this->getID().toString();
    }

    return l_Ret;
}

// I_Configurable
void ImplementationGraphicsItem::openConfiguration()
{
    this->getConfig<ImplementationConfiguration>().registerDiagram(this->getDiagramContainer());
    this->getConfig<ImplementationConfiguration>().setListener(this->getSelectableType(), this->getID());

    this->getConfig<ImplementationConfiguration>().setAutoRoute(!this->getPathIsForced());

    // Let's rock
    ConfigWidget::open(&this->getConfig<ImplementationConfiguration>());
}
void ImplementationGraphicsItem::closeConfiguration()
{
    ConfigWidget::close();
}
void ImplementationGraphicsItem::applyConfiguration()
{
    this->setPathIsForced(!this->getConfig<ImplementationConfiguration>().getAutoRoute());

    if( false == this->getPathIsForced() )
    {
        this->route(this->getPath().first(),
                    this->getPath().last());
    }

    this->getDiagramContainer()->changed(this);

    this->closeConfiguration();
}

void ImplementationGraphicsItem::setupArrow()
{
    static const unsigned short ARROW_WIDTH = 10U;
    static const unsigned short ARROW_DEPTH = 10U;
    static const unsigned short IMPLEMENTATION_THICKNESS = 4U;
    static const Qt::GlobalColor IMPLEMENTATION_COLOR = Qt::black;

    // The middle point is the point in the middle of the tip, only for computation ease
    qreal l_lastLineAngle = this->getLastLineAngle();
    QPointF l_MiddlePoint;
    l_MiddlePoint.setX(this->getPath().last().x() - (qCos(l_lastLineAngle)*ARROW_DEPTH));
    l_MiddlePoint.setY(this->getPath().last().y() + (qSin(l_lastLineAngle)*ARROW_DEPTH));

    QList<QPointF> l_Points;
    // 3 points are needed to define the arrow
    l_Points.append(this->getPath().last()); // first point is end of line
    qreal l_currentPointAngle = l_lastLineAngle + (M_PI/2.0);
    l_Points.append(QPointF(l_MiddlePoint.x() + qCos(l_currentPointAngle)*ARROW_WIDTH,
                    l_MiddlePoint.y() - qSin(l_currentPointAngle)*ARROW_WIDTH));
    l_currentPointAngle += (M_PI);
    l_Points.append(QPointF(l_MiddlePoint.x() + qCos(l_currentPointAngle)*ARROW_WIDTH,
                    l_MiddlePoint.y() - qSin(l_currentPointAngle)*ARROW_WIDTH));

    for( unsigned short i_arrow=0U; i_arrow < m_Arrow.count(); i_arrow++ )
    {
        this->removeFromGroup(m_Arrow[i_arrow]);
        delete m_Arrow[i_arrow];
    }
    m_Arrow.clear();

    m_Arrow.append(new QGraphicsLineItem(QLineF(l_Points[0], l_Points[1])));
    m_Arrow.last()->setPen(QPen(IMPLEMENTATION_COLOR, IMPLEMENTATION_THICKNESS));
    this->addToGroup(m_Arrow.last());
    m_Arrow.append(new QGraphicsLineItem(QLineF(l_Points[0], l_Points[2])));
    m_Arrow.last()->setPen(QPen(IMPLEMENTATION_COLOR, IMPLEMENTATION_THICKNESS));
    this->addToGroup(m_Arrow.last());
}

void ImplementationGraphicsItem::refreshDisplay()
{
    this->clear();

    GraphicConnector::refreshDisplay();

    QPen l_LinesPen(Qt::PenStyle::DashLine);
    l_LinesPen.setWidth(this->getThickness());
    this->setLinesPen(l_LinesPen);

    this->setupArrow();
}

void ImplementationGraphicsItem::clear()
{
    GraphicConnector::clear();

    for( unsigned short i_arrow=0U; i_arrow < m_Arrow.count(); i_arrow++ )
    {
        this->removeFromGroup(m_Arrow[i_arrow]);
        delete m_Arrow[i_arrow];
    }
    m_Arrow.clear();
}
