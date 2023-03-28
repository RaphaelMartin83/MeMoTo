#include "HandleGraphicsItem.h"
#include "I_Connectable.h"
#include "ConnectionDefinitions.h"

#include <QBrush>

static const unsigned short DEFAULT_HANDLES_DIAMETER = 10U;
static const float SELECTION_GROWING_FACTOR = 1.5F;

static const Qt::GlobalColor DEFAULT_HANDLE_COLOR = Qt::black;
static const Qt::GlobalColor DEFAULT_HANDLE_SELECTED_COLOR = Qt::red;
static const Qt::GlobalColor DEFAULT_HANDLE_CONNECTION_COLOR = Qt::magenta;

HandleGraphicsItem::HandleGraphicsItem(
        const QPoint& p_Position, const eConnectDirection& p_HandlePosition,
        bool p_VisibleUnselected):
    I_Selectable(p_Position)
  , m_isSelected(false)
  , m_isVisibleUnselected(p_VisibleUnselected)
{
    m_Circle = new QGraphicsEllipseItem(QRect(
                          p_Position.x() - DEFAULT_HANDLES_DIAMETER/2,
                          p_Position.y() - DEFAULT_HANDLES_DIAMETER/2,
                          DEFAULT_HANDLES_DIAMETER,
                          DEFAULT_HANDLES_DIAMETER));
    m_Circle->setBrush(DEFAULT_HANDLE_COLOR);

    this->addToGroup(m_Circle);
    this->setVisible(m_isVisibleUnselected);

    m_HandlePosition = p_HandlePosition;
}

HandleGraphicsItem::~HandleGraphicsItem()
{
}

void HandleGraphicsItem::setHandlePosition(const eConnectDirection& p_HandlePos)
{
    m_HandlePosition = p_HandlePos;
}

const eConnectDirection& HandleGraphicsItem::getHandlePosition() const
{
    return m_HandlePosition;
}

QPoint HandleGraphicsItem::getHandleCoordinates() const
{
    return this->getPos();
}

bool HandleGraphicsItem::isItYou(QPoint p_Pos) const
{
    bool l_ret;

    if( abs(p_Pos.x() - this->getPos().x()) < DEFAULT_HANDLES_DIAMETER &&
            abs(p_Pos.y() - this->getPos().y()) < DEFAULT_HANDLES_DIAMETER )
    {
        l_ret = true;
    }
    else
    {
        l_ret = false;
    }

    return l_ret;
}

void HandleGraphicsItem::selectForConnection()
{
    m_Circle->setRect(QRect(
                          this->getPos().x() - (DEFAULT_HANDLES_DIAMETER/2) * SELECTION_GROWING_FACTOR,
                          this->getPos().y() - (DEFAULT_HANDLES_DIAMETER/2) * SELECTION_GROWING_FACTOR,
                          DEFAULT_HANDLES_DIAMETER * SELECTION_GROWING_FACTOR,
                          DEFAULT_HANDLES_DIAMETER * SELECTION_GROWING_FACTOR));
    m_Circle->setBrush(DEFAULT_HANDLE_CONNECTION_COLOR);
    this->setVisible(true);
}

void HandleGraphicsItem::select()
{
    m_Circle->setBrush(DEFAULT_HANDLE_SELECTED_COLOR);

    this->removeFromGroup(m_Circle);
    this->addToGroup(m_Circle);
    this->setVisible(true);

    m_isSelected = true;
}
void HandleGraphicsItem::select(QPoint p_Pos)
{
    this->select();
}

void HandleGraphicsItem::unselect()
{
    m_Circle->setRect(QRect(
                          this->getPos().x() - (DEFAULT_HANDLES_DIAMETER/2),
                          this->getPos().y() - (DEFAULT_HANDLES_DIAMETER/2),
                          DEFAULT_HANDLES_DIAMETER,
                          DEFAULT_HANDLES_DIAMETER));
    m_Circle->setBrush(DEFAULT_HANDLE_COLOR);

    this->removeFromGroup(m_Circle);
    this->addToGroup(m_Circle);
    this->setVisible(m_isVisibleUnselected);

    m_isSelected = false;
}

void HandleGraphicsItem::move(QPoint p_Pos)
{
    this->setPos(p_Pos);
}

bool HandleGraphicsItem::isFullySelected() const
{
    return m_isSelected;
}

QPoint HandleGraphicsItem::getFocusPosition() const
{
    return this->getPos();
}

void HandleGraphicsItem::refreshDisplay()
{
    m_Circle->setZValue(getDepthFromPlan(this->getPlan()));
}

QString HandleGraphicsItem::getSelectableType() const
{
    // Handles cannot be searched for
    return QString();
}
QStringList HandleGraphicsItem::getSearchFields() const
{
    return QStringList();
}
QString HandleGraphicsItem::getDataFromField(const QString& p_FieldName) const
{
    return QString();
}
