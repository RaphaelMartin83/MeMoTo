#include "HumanReadableColor.h"

HumanReadableColor::HumanReadableColor(const QColor& p_Color):
    QColor(p_Color)
{

}

QString HumanReadableColor::getHumanReadableColor()
{
    QStringList l_ColorNames = QColor::colorNames();
    QString l_Ret = "";

    for( unsigned int i_colors = 0U;
         (i_colors < l_ColorNames.count()) && (l_Ret == "");
         i_colors++ )
    {
        QColor l_ColorToCompare(l_ColorNames[i_colors]);

        if( *this == l_ColorToCompare )
        {
            l_Ret = l_ColorNames[i_colors];
        }
    }

    return l_Ret;
}
