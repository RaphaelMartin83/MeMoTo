#ifndef HUMANREADABLECOLOR_H
#define HUMANREADABLECOLOR_H

#include <QColor>

class HumanReadableColor: public QColor
{
public:
    HumanReadableColor(const QColor& p_Color);

    QString getHumanReadableColor();
};

#endif // HUMANREADABLECOLOR_H
