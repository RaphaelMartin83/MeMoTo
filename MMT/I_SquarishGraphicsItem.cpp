#include <QBrush>
#include <QTextDocument>
#include <QTextOption>

#include "I_SquarishGraphicsItem.h"

#include "HumanReadableColor.h"

static const unsigned short DEFAULT_HANDLES_DIAMETER = 8U;
static const unsigned short MIN_DIST_BETWEEN_HANDLES = static_cast<unsigned short>(GridReferential::getGridDistance(30));
static const unsigned short SQUARE_CORNERS_SELECTION_THICKNESS = 30U;
static const unsigned short SQUARE_TEXT_LABELS_HEIGHT = 15U;
static const unsigned short SQUARE_CLEARANCE = static_cast<unsigned short>(GridReferential::getGridDistance(100U));

I_SquarishGraphicsItem::I_SquarishGraphicsItem(QPointF p_Pos, const QString& p_Name,
                                               unsigned int p_ID,
                                              unsigned short p_Width,
                                              unsigned short p_Height):
    m_Rect(nullptr)
  , m_Handles()
  , m_SelectedHandlesDirection(eConnectionDirection_None)
  , m_SelectedHandleForConnectionFrom(nullptr)
  , m_SelectedHandleForConnectionTo(nullptr)
  , m_Name(p_Name)
{
    this->setID(p_ID);

    m_Width = GridReferential::getGridDistance(p_Width);
    m_Height = GridReferential::getGridDistance(p_Height);

    // Upon consruction, set this item to default configuration
    m_Pos.setX( p_Pos.x() - m_Width/2 );
    m_Pos.setY( p_Pos.y() - m_Height/2 );
    m_Pos = GridReferential::getPointOnGrid(m_Pos);
    m_Rect = new QGraphicsRectItem(QRect(
                                       m_Pos.x(), m_Pos.y(),
                                       m_Width,
                                       m_Height));
    m_Rect->setBrush(QColor("blanchedalmond"));
    this->addToGroup(m_Rect);

    m_NameGraphicsItem = new QGraphicsTextItem(m_Name);
    m_NameGraphicsItem->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
    this->addToGroup(m_NameGraphicsItem);

    m_LineUnderTitle = new QGraphicsLineItem();
    this->addToGroup(m_LineUnderTitle);

    m_LeftDoubleArrow = new DoubleArrowGraphicsItem();
    m_LeftDoubleArrow->setColor(Qt::red);
    m_LeftDoubleArrow->setDirection(eArrowDirection_Horizontal);
    m_LeftDoubleArrow->setLength(20U);
    m_LeftDoubleArrow->setWidth(6U);
    m_LeftDoubleArrow->setVisible(false);
    this->addToGroup(m_LeftDoubleArrow);

    m_RightDoubleArrow = new DoubleArrowGraphicsItem();
    m_RightDoubleArrow->setColor(Qt::red);
    m_RightDoubleArrow->setDirection(eArrowDirection_Horizontal);
    m_RightDoubleArrow->setLength(20U);
    m_RightDoubleArrow->setWidth(6U);
    m_RightDoubleArrow->setVisible(false);
    this->addToGroup(m_RightDoubleArrow);

    m_TopDoubleArrow = new DoubleArrowGraphicsItem();
    m_TopDoubleArrow->setColor(Qt::red);
    m_TopDoubleArrow->setDirection(eArrowDirection_Vertical);
    m_TopDoubleArrow->setLength(20U);
    m_TopDoubleArrow->setWidth(6U);
    m_TopDoubleArrow->setVisible(false);
    this->addToGroup(m_TopDoubleArrow);

    m_BotDoubleArrow = new DoubleArrowGraphicsItem();
    m_BotDoubleArrow->setColor(Qt::red);
    m_BotDoubleArrow->setDirection(eArrowDirection_Vertical);
    m_BotDoubleArrow->setLength(20U);
    m_BotDoubleArrow->setWidth(6U);
    m_BotDoubleArrow->setVisible(false);
    this->addToGroup(m_BotDoubleArrow);

    I_SquarishGraphicsItem::refreshDisplay();
}

I_SquarishGraphicsItem::I_SquarishGraphicsItem(const QJsonObject& p_Json):
    I_SquarishGraphicsItem(QPointF(p_Json.find("PositionX")->toDouble(),
                              p_Json.find("PositionY")->toDouble()),
                      p_Json.find("Name")->toString(),
                      p_Json.find("ID")->toInt(),
                      p_Json.find("Width")->toInt(),
                      p_Json.find("Height")->toInt())
{
    QString l_JsonColor = p_Json.find("Color")->toString();
    if("" != l_JsonColor)
    {
        this->setColor(QColor(l_JsonColor));
    }
}

I_SquarishGraphicsItem::~I_SquarishGraphicsItem()
{
    this->removeFromGroup(m_Rect);
    delete m_Rect;

    this->removeFromGroup(m_NameGraphicsItem);
    delete m_NameGraphicsItem;

    this->removeFromGroup(m_LineUnderTitle);
    delete m_LineUnderTitle;

    deleteHandles();
}

void I_SquarishGraphicsItem::refreshDisplay()
{
    m_Rect->setRect(QRect(
                       m_Pos.x(), m_Pos.y(),
                       m_Width,
                       m_Height));
    m_NameGraphicsItem->setPos(m_Pos);
    m_NameGraphicsItem->setTextWidth(m_Width);
    m_NameGraphicsItem->setPlainText(m_Name);
    m_LineUnderTitle->setLine(QLine(m_Pos.x(), m_Pos.y() + SQUARE_TEXT_LABELS_HEIGHT*2,
                                             m_Pos.x() + m_Width, m_Pos.y() + SQUARE_TEXT_LABELS_HEIGHT*2));

    m_LeftDoubleArrow->setPosition(QPoint(m_Pos.x(), m_Pos.y() + m_Height/2));
    m_RightDoubleArrow->setPosition(QPoint(m_Pos.x() + m_Width, m_Pos.y() + m_Height/2));
    m_TopDoubleArrow->setPosition(QPoint(m_Pos.x() + m_Width/2, m_Pos.y()));
    m_BotDoubleArrow->setPosition(QPoint(m_Pos.x() + m_Width/2, m_Pos.y() + m_Height));

    this->deleteHandles();
    this->setupHandles();
    this->setZValue(getDepthFromPlan(this->getPlan()));

    if( true == m_isFullySelected )
    {
        this->selectAllHandles();
    }
    else
    {
        this->selectHandlesOnSide(m_SelectedHandlesDirection);
    }
}


void I_SquarishGraphicsItem::setWidth(unsigned short p_Width)
{
    m_Width = GridReferential::getGridDistance(p_Width);
    m_Rect->setRect(QRect(
                       m_Pos.x(), m_Pos.y(),
                       m_Width,
                       m_Height));
}
void I_SquarishGraphicsItem::setHeight(unsigned short p_Height)
{
    m_Height = GridReferential::getGridDistance(p_Height);
    m_Rect->setRect(QRect(
                       m_Pos.x(), m_Pos.y(),
                       m_Width,
                       m_Height));
}
unsigned short I_SquarishGraphicsItem::getWidth()
{
    return m_Width;
}
unsigned short I_SquarishGraphicsItem::getHeight()
{
    return m_Height;
}

void I_SquarishGraphicsItem::setName(const QString& p_Name)
{
    m_Name = p_Name;
    m_NameGraphicsItem->setPlainText(m_Name);
}
const QString& I_SquarishGraphicsItem::getName() const
{
    return m_Name;
}
void I_SquarishGraphicsItem::setColor(QColor p_Color)
{
    m_Rect->setBrush(p_Color);
}
QColor I_SquarishGraphicsItem::getColor() const
{
    return m_Rect->brush().color();
}
QString I_SquarishGraphicsItem::getColorName() const
{
    HumanReadableColor l_Color(this->getColor());
    return l_Color.getHumanReadableColor();
}

QPoint I_SquarishGraphicsItem::getPos() const
{
    QPoint l_Pos(m_Pos.x() + m_Width/2,
                 m_Pos.y() + m_Height/2);
    return l_Pos;
}
void I_SquarishGraphicsItem::setPos(const QPoint& p_Pos)
{
    QPoint l_Before = this->getPos();
    m_Pos.setX(p_Pos.x() - m_Width/2);
    m_Pos.setY(p_Pos.y() - m_Height/2);
    m_Pos = GridReferential::getPointOnGrid(m_Pos);
    QPoint l_After = this->getPos();
    this->moveChildren(l_Before, l_After);

    this->refreshDisplay();
}

void I_SquarishGraphicsItem::setupHandles()
{
    QPoint l_HandleCoordinates;

    // Top left handle
    l_HandleCoordinates.setX(m_Pos.x());
    l_HandleCoordinates.setY(m_Pos.y());
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_TopLeft));
    this->addToGroup(m_Handles.last());

    // Top right
    l_HandleCoordinates.setX(m_Pos.x() + m_Width);
    l_HandleCoordinates.setY(m_Pos.y());
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_TopRight));
    this->addToGroup(m_Handles.last());

    // Bot left
    l_HandleCoordinates.setX(m_Pos.x());
    l_HandleCoordinates.setY(m_Pos.y() + m_Height);
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_BotLeft));
    this->addToGroup(m_Handles.last());

    // Bot right
    l_HandleCoordinates.setX(m_Pos.x() + m_Width);
    l_HandleCoordinates.setY(m_Pos.y() + m_Height);
    m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_BotRight));
    this->addToGroup(m_Handles.last());

    // Set handles along state width
    for( unsigned short i_wHandles = MIN_DIST_BETWEEN_HANDLES;
         i_wHandles < m_Width; i_wHandles+= MIN_DIST_BETWEEN_HANDLES )
    {
        // Top intermediate handles
        l_HandleCoordinates.setX(m_Pos.x() + i_wHandles);
        l_HandleCoordinates.setY(m_Pos.y());
        m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Top));
        this->addToGroup(m_Handles.last());
    }

    for( unsigned short i_wHandles = MIN_DIST_BETWEEN_HANDLES;
         i_wHandles < m_Width; i_wHandles+= MIN_DIST_BETWEEN_HANDLES )
    {
        // Bot intermediate handles
        l_HandleCoordinates.setX(m_Pos.x() + i_wHandles);
        l_HandleCoordinates.setY(m_Pos.y() + m_Height);
        m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Bottom));
        this->addToGroup(m_Handles.last());
    }

    // Set handles along state height
    for( unsigned short i_hHandles = MIN_DIST_BETWEEN_HANDLES;
         i_hHandles < m_Height; i_hHandles+=MIN_DIST_BETWEEN_HANDLES )
    {
        // Left intermediate handles
        l_HandleCoordinates.setX(m_Pos.x());
        l_HandleCoordinates.setY(m_Pos.y() + i_hHandles);
        m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Left));
        this->addToGroup(m_Handles.last());
    }

    for( unsigned short i_hHandles = MIN_DIST_BETWEEN_HANDLES;
         i_hHandles < m_Height; i_hHandles+=MIN_DIST_BETWEEN_HANDLES )
    {
        // Right intermediate handles
        l_HandleCoordinates.setX(m_Pos.x() + m_Width);
        l_HandleCoordinates.setY(m_Pos.y() + i_hHandles);
        m_Handles.append(new HandleGraphicsItem(l_HandleCoordinates, eConnectionDirection_Right));
        this->addToGroup(m_Handles.last());
    }
}

void I_SquarishGraphicsItem::deleteHandles()
{
    for(unsigned int i_handles = 0U; i_handles < m_Handles.count(); i_handles++ )
    {
        this->removeFromGroup(m_Handles[i_handles]);
        delete m_Handles[i_handles];
    }
    m_Handles.clear();
}

const QList<HandleGraphicsItem*>& I_SquarishGraphicsItem::getConnectableHandles() const
{
    return m_Handles;
}
QRect I_SquarishGraphicsItem::getCollisionRectangle() const
{
    return QRect(m_Pos.x(), m_Pos.y(), m_Width, m_Height);
}
QString I_SquarishGraphicsItem::getConnectableName() const
{
    return m_Name;
}

void I_SquarishGraphicsItem::select()
{
    this->selectAllHandles();
    m_isFullySelected = true;
}
void I_SquarishGraphicsItem::select(QPoint p_Pos)
{
    bool l_found = false;
    m_isFullySelected = false;

    // I'm a container
    if(this->selectChild(p_Pos))
    {
        return;
    }

    // Virtual selection corners
    if( (p_Pos.x() > m_Pos.x()) &&
            (p_Pos.x() < (m_Pos.x() + SQUARE_CORNERS_SELECTION_THICKNESS)) &&
            (p_Pos.y() > (m_Pos.y() + SQUARE_CORNERS_SELECTION_THICKNESS)) &&
            (p_Pos.y() < (m_Pos.y() + m_Height - SQUARE_CORNERS_SELECTION_THICKNESS)) )
    {
        // Left
        m_SelectedHandlesDirection = eConnectionDirection_Left;
        m_LeftDoubleArrow->setVisible(true);
        l_found = true;
    }
    else if( (p_Pos.x() > (m_Pos.x() + m_Width - SQUARE_CORNERS_SELECTION_THICKNESS)) &&
             (p_Pos.x() < (m_Pos.x() + m_Width)) &&
             (p_Pos.y() > (m_Pos.y() + SQUARE_CORNERS_SELECTION_THICKNESS)) &&
             (p_Pos.y() < (m_Pos.y() + m_Height - SQUARE_CORNERS_SELECTION_THICKNESS)) )
    {
        // Right
        m_SelectedHandlesDirection = eConnectionDirection_Right;
        m_RightDoubleArrow->setVisible(true);
        l_found = true;
    }
    else if( (p_Pos.x() > (m_Pos.x() + SQUARE_CORNERS_SELECTION_THICKNESS)) &&
              (p_Pos.x() < (m_Pos.x() + m_Width - SQUARE_CORNERS_SELECTION_THICKNESS)) &&
              (p_Pos.y() > m_Pos.y()) &&
              (p_Pos.y() < (m_Pos.y() + SQUARE_CORNERS_SELECTION_THICKNESS)) )
    {
        // Top
        m_SelectedHandlesDirection = eConnectionDirection_Top;
        m_TopDoubleArrow->setVisible(true);
        l_found = true;
    }
    else if( (p_Pos.x() > (m_Pos.x() + SQUARE_CORNERS_SELECTION_THICKNESS)) &&
              (p_Pos.x() < (m_Pos.x() + m_Width - SQUARE_CORNERS_SELECTION_THICKNESS)) &&
              (p_Pos.y() > (m_Pos.y() + m_Height - SQUARE_CORNERS_SELECTION_THICKNESS)) &&
              (p_Pos.y() < (m_Pos.y() + m_Height)) )
    {
        // Bottom
        m_SelectedHandlesDirection = eConnectionDirection_Bottom;
        m_BotDoubleArrow->setVisible(true);
        l_found = true;
    }
    else if( (p_Pos.x() > m_Pos.x()) &&
             (p_Pos.x() < (m_Pos.x() + SQUARE_CORNERS_SELECTION_THICKNESS)) &&
             (p_Pos.y() > m_Pos.y()) &&
             (p_Pos.y() < (m_Pos.y() + SQUARE_CORNERS_SELECTION_THICKNESS)) )
    {
        // Top Left
        m_SelectedHandlesDirection = eConnectionDirection_TopLeft;
        m_TopDoubleArrow->setVisible(true);
        m_LeftDoubleArrow->setVisible(true);
        l_found = true;
    }
    else if( (p_Pos.x() > (m_Pos.x() + m_Width - SQUARE_CORNERS_SELECTION_THICKNESS)) &&
             (p_Pos.x() < (m_Pos.x() + m_Width)) &&
             (p_Pos.y() > m_Pos.y()) &&
             (p_Pos.y() < (m_Pos.y() + SQUARE_CORNERS_SELECTION_THICKNESS)) )
    {
        // Top Right
        m_SelectedHandlesDirection = eConnectionDirection_TopRight;
        m_TopDoubleArrow->setVisible(true);
        m_RightDoubleArrow->setVisible(true);
        l_found = true;
    }
    else if( (p_Pos.x() > m_Pos.x()) &&
             (p_Pos.x() < (m_Pos.x() + SQUARE_CORNERS_SELECTION_THICKNESS)) &&
              (p_Pos.y() > (m_Pos.y() + m_Height - SQUARE_CORNERS_SELECTION_THICKNESS)) &&
              (p_Pos.y() < (m_Pos.y() + m_Height)) )
    {
        // Bot Left
        m_SelectedHandlesDirection = eConnectionDirection_BotLeft;
        m_BotDoubleArrow->setVisible(true);
        m_LeftDoubleArrow->setVisible(true);
        l_found = true;
    }
    else if( (p_Pos.x() > (m_Pos.x() + m_Width - SQUARE_CORNERS_SELECTION_THICKNESS)) &&
             (p_Pos.x() < (m_Pos.x() + m_Width)) &&
              (p_Pos.y() > (m_Pos.y() + m_Height - SQUARE_CORNERS_SELECTION_THICKNESS)) &&
              (p_Pos.y() < (m_Pos.y() + m_Height)) )
    {
        // Bottom Right
        m_SelectedHandlesDirection = eConnectionDirection_BotRight;
        m_BotDoubleArrow->setVisible(true);
        m_RightDoubleArrow->setVisible(true);
        l_found = true;
    }
    else
    {
        m_SelectedHandlesDirection = eConnectionDirection_None;
    }

    if( false == l_found && this->m_Rect->contains(p_Pos) )
    {
        this->selectAllHandles();
        m_isFullySelected = true;
    }
}

void I_SquarishGraphicsItem::selectAllHandles()
{
    m_LeftDoubleArrow->setVisible(true);
    m_RightDoubleArrow->setVisible(true);
    m_TopDoubleArrow->setVisible(true);
    m_BotDoubleArrow->setVisible(true);

    m_isFullySelected = true;
}

void I_SquarishGraphicsItem::selectHandlesOnSide(eConnectDirection p_Direction)
{
    switch(m_SelectedHandlesDirection)
    {
    case eConnectionDirection_Left:
    {
        m_SelectedHandlesDirection = eConnectionDirection_Left;
        m_LeftDoubleArrow->setVisible(true);
        break;
    }
    case eConnectionDirection_Right:
    {
        m_SelectedHandlesDirection = eConnectionDirection_Right;
        m_RightDoubleArrow->setVisible(true);
        break;
    }
    case eConnectionDirection_Top:
    {
        m_SelectedHandlesDirection = eConnectionDirection_Top;
        m_TopDoubleArrow->setVisible(true);
        break;
    }
    case eConnectionDirection_Bottom:
    {
        m_SelectedHandlesDirection = eConnectionDirection_Bottom;
        m_BotDoubleArrow->setVisible(true);
        break;
    }
    case eConnectionDirection_TopLeft:
    {
        m_SelectedHandlesDirection = eConnectionDirection_TopLeft;
        m_TopDoubleArrow->setVisible(true);
        m_LeftDoubleArrow->setVisible(true);
        break;
    }
    case eConnectionDirection_TopRight:
    {
        m_SelectedHandlesDirection = eConnectionDirection_TopRight;
        m_TopDoubleArrow->setVisible(true);
        m_RightDoubleArrow->setVisible(true);
        break;
    }
    case eConnectionDirection_BotLeft:
    {
        m_SelectedHandlesDirection = eConnectionDirection_BotLeft;
        m_BotDoubleArrow->setVisible(true);
        m_LeftDoubleArrow->setVisible(true);
        break;
    }
    case eConnectionDirection_BotRight:
    {
        m_SelectedHandlesDirection = eConnectionDirection_BotRight;
        m_BotDoubleArrow->setVisible(true);
        m_RightDoubleArrow->setVisible(true);
        break;
    }
    default:
    {
        break;
    }
    }
}

bool I_SquarishGraphicsItem::isFullySelected() const
{
    return m_isFullySelected;
}

QPoint I_SquarishGraphicsItem::getFocusPosition() const
{
    return this->getPos();
}

void I_SquarishGraphicsItem::unselect()
{
    this->unselectChild();

    m_LeftDoubleArrow->setVisible(false);
    m_RightDoubleArrow->setVisible(false);
    m_TopDoubleArrow->setVisible(false);
    m_BotDoubleArrow->setVisible(false);

    m_isFullySelected = false;
    m_SelectedHandlesDirection = eConnectionDirection_None;
}

void I_SquarishGraphicsItem::move(QPoint p_Pos)
{
    if( true == m_isFullySelected )
    {
        QPoint l_Before = this->getPos();
        this->translateTo(p_Pos);
        QPoint l_After = this->getPos();
        this->moveChildren(l_Before, l_After);
    }
    else
    {
        this->moveChild(p_Pos);
        this->resizeTo(p_Pos);
    }
}

bool I_SquarishGraphicsItem::isItYou(QPoint p_Pos) const
{
    bool l_isItMe = false;

    // First, check each handle perimeter
    for(unsigned short i_handles=0U;
        (i_handles < m_Handles.count()) && (false == l_isItMe);
        i_handles++ )
    {
        l_isItMe = m_Handles[i_handles]->isItYou(p_Pos);
    }

    l_isItMe = m_Rect->contains(p_Pos);

    return l_isItMe;
}

void I_SquarishGraphicsItem::translateTo(const QPoint& p_Pos)
{
    m_Pos.setX( GridReferential::getGridDistance(p_Pos.x() - m_Width/2) );
    m_Pos.setY( GridReferential::getGridDistance(p_Pos.y() - m_Height/2) );

    this->refreshDisplay();
}

void I_SquarishGraphicsItem::resizeTo(const QPoint& p_Pos)
{
    // Depending on the direction of the selected handle, resize in different directions
    switch(m_SelectedHandlesDirection)
    {
    case eConnectionDirection_Left:
    {
        // Modifies size and position
        this->setWidth(m_Width + (m_Pos.x() - p_Pos.x()));
        m_Pos.setX( GridReferential::getGridDistance(p_Pos.x()) );
        this->refreshDisplay();
        break;
    }
    case eConnectionDirection_Right:
    {
        this->setWidth(m_Width + (p_Pos.x() - m_Pos.x() - m_Width));
        m_Pos.setX( GridReferential::getGridDistance(p_Pos.x() - m_Width) );
        this->refreshDisplay();
        break;
    }
    case eConnectionDirection_Top:
    {
        this->setHeight(m_Height + (m_Pos.y() - p_Pos.y()));
        m_Pos.setY( GridReferential::getGridDistance(p_Pos.y()) );
        this->refreshDisplay();
        break;
    }
    case eConnectionDirection_Bottom:
    {
        this->setHeight(m_Height + (p_Pos.y() - m_Pos.y() - m_Height));
        m_Pos.setY( GridReferential::getGridDistance(p_Pos.y() - m_Height) );
        this->refreshDisplay();
        break;
    }
    case eConnectionDirection_TopLeft:
    {
        this->setWidth(m_Width + (m_Pos.x() - p_Pos.x()));
        m_Pos.setX( GridReferential::getGridDistance(p_Pos.x()) );
        this->setHeight(m_Height + (m_Pos.y() - p_Pos.y()));
        m_Pos.setY( GridReferential::getGridDistance(p_Pos.y()) );
        this->refreshDisplay();
        break;
    }
    case eConnectionDirection_TopRight:
    {
        this->setHeight(m_Height + (m_Pos.y() - p_Pos.y()));
        m_Pos.setY( GridReferential::getGridDistance(p_Pos.y()) );
        this->setWidth(m_Width + (p_Pos.x() - m_Pos.x() - m_Width));
        m_Pos.setX( GridReferential::getGridDistance(p_Pos.x() - m_Width) );
        this->refreshDisplay();
        break;
    }
    case eConnectionDirection_BotLeft:
    {
        this->setHeight(m_Height + (p_Pos.y() - m_Pos.y() - m_Height));
        m_Pos.setY( GridReferential::getGridDistance(p_Pos.y() - m_Height) );
        this->setWidth(m_Width + (m_Pos.x() - p_Pos.x()));
        m_Pos.setX( GridReferential::getGridDistance(p_Pos.x()) );
        this->refreshDisplay();
        break;
    }
    case eConnectionDirection_BotRight:
    {
        this->setHeight(m_Height + (p_Pos.y() - m_Pos.y() - m_Height));
        m_Pos.setY( GridReferential::getGridDistance(p_Pos.y() - m_Height) );
        this->setWidth(m_Width + (p_Pos.x() - m_Pos.x() - m_Width));
        m_Pos.setX( GridReferential::getGridDistance(p_Pos.x() - m_Width) );
        this->refreshDisplay();
        break;
    }
    default:
    {
        break;
    }
    }
}

void I_SquarishGraphicsItem::resizeToContain(QRect p_Rect)
{
    if( m_Pos.x() > (p_Rect.left() - SQUARE_CLEARANCE) )
    {
        this->setWidth(m_Width + (m_Pos.x() - p_Rect.left() + SQUARE_CLEARANCE));
        m_Pos.setX(p_Rect.left() - SQUARE_CLEARANCE);
    }
    if( m_Pos.y() > (p_Rect.top() - SQUARE_CLEARANCE) )
    {
        this->setWidth(m_Height + (m_Pos.y() - p_Rect.top() + SQUARE_CLEARANCE));
        m_Pos.setY(p_Rect.top() - SQUARE_CLEARANCE);
    }
    if( (m_Pos.x() + m_Width) < (p_Rect.right() + SQUARE_CLEARANCE) )
    {
        this->setWidth(p_Rect.right() + SQUARE_CLEARANCE - m_Pos.x());
    }
    if( (m_Pos.y() + m_Height) < (p_Rect.bottom() + SQUARE_CLEARANCE) )
    {
        this->setHeight(p_Rect.bottom() + SQUARE_CLEARANCE - m_Pos.y());
    }

    this->refreshDisplay();
}

void I_SquarishGraphicsItem::resizeToFitContent()
{
    unsigned int l_minX = 0U;
    unsigned int l_minY = 0U;
    unsigned int l_maxX = 0U;
    unsigned int l_maxY = 0U;

    QList<I_Selectable*> l_Selectables = this->getAllSelectables();
    for( unsigned int i_sel = 0U; i_sel < l_Selectables.count(); i_sel++ )
    {
        QRect l_BoundingRect = l_Selectables[i_sel]->getCollisionRectangle();
        if((l_minX > l_BoundingRect.left()) || (0U == i_sel))
        {
            l_minX = l_BoundingRect.left();
        }
        if((l_minY > l_BoundingRect.top()) || (0U == i_sel))
        {
            l_minY = l_BoundingRect.top();
        }
        if((l_maxX < l_BoundingRect.right()) || (0U == i_sel))
        {
            l_maxX = l_BoundingRect.right();
        }
        if((l_maxY < l_BoundingRect.bottom()) || (0U == i_sel))
        {
            l_maxY = l_BoundingRect.bottom();
        }
    }

    resizeToContain(QRect(l_minX, l_minY, l_maxX - l_minX, l_maxY - l_minY));
}

QJsonObject I_SquarishGraphicsItem::toJson()
{
    QJsonObject l_MyJson = I_ContainersContainer::toJson();

    l_MyJson.insert("ID", static_cast<qint64>(this->getID()));
    l_MyJson.insert("Name", this->getName());
    l_MyJson.insert("PositionX", this->getPos().x());
    l_MyJson.insert("PositionY", this->getPos().y());
    l_MyJson.insert("Width", this->getWidth());
    l_MyJson.insert("Height", this->getHeight());
    l_MyJson.insert("Color", this->getColorName());

    return l_MyJson;
}

void I_SquarishGraphicsItem::fromJson(QJsonObject p_Json)
{
}

void I_SquarishGraphicsItem::setPlan(unsigned short p_Plan)
{
    I_GraphicsItem::setPlan(p_Plan);

    this->setZValue(DEFAULT_Z_DEPTH - p_Plan + 1U);
}
