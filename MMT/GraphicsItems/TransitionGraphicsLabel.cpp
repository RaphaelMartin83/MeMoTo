#include <QFont>
#include <QFontMetrics>

#include "TransitionGraphicsLabel.h"

static const unsigned short TRANSITION_LABEL_PIXELS_PER_LINE = 20U;
static const float TRANSITION_LABEL_CHARACTER_SIZE_FACTOR = 100.0F;

TransitionGraphicsLabel::TransitionGraphicsLabel():
    QGraphicsTextItem()
{
    this->setDefaultTextColor(Qt::GlobalColor::darkGray);
}

QString TransitionGraphicsLabel::getDisplayedString() const
{
    QString l_LabelString;

    if("" != m_Guard)
    {
        l_LabelString += "[" + m_Guard + "]";
    }
    l_LabelString += m_Event;
    if("" != m_Action)
    {
        l_LabelString += "/" + m_Action;
    }

    return l_LabelString;
}

QRectF TransitionGraphicsLabel::boundingRect() const
{
    QFontMetrics l_fontMetrics(this->font());
    QRectF l_Rect(0.0,
                  0.0,
                  this->getDisplayedString().length() * l_fontMetrics.averageCharWidth() + 10,
                  TRANSITION_LABEL_PIXELS_PER_LINE * 2U);

    return l_Rect;
}

void TransitionGraphicsLabel::setEvent(const QString& p_Event)
{
    m_Event = p_Event;
    this->setPlainText(this->getDisplayedString());
}
void TransitionGraphicsLabel::setAction(const QString& p_Action)
{
    m_Action = p_Action;
    this->setPlainText(this->getDisplayedString());
}
void TransitionGraphicsLabel::setGuard(const QString& p_Guard)
{
    m_Guard = p_Guard;
    this->setPlainText(this->getDisplayedString());
}
void TransitionGraphicsLabel::setStereotype(const QString& p_Stereotype)
{
    m_Stereotype = p_Stereotype;
    this->setPlainText(this->getDisplayedString());
}
