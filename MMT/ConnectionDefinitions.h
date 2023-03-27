#ifndef CONNECTIONDEFINITIONS_H
#define CONNECTIONDEFINITIONS_H

// The way connector shall approach the connected element from
typedef enum
{
    eConnectionDirection_None,
    eConnectionDirection_Left,
    eConnectionDirection_Right,
    eConnectionDirection_Top,
    eConnectionDirection_Bottom,
    eConnectionDirection_TopLeft,
    eConnectionDirection_TopRight,
    eConnectionDirection_BotLeft,
    eConnectionDirection_BotRight,
    eConnectionDirection_Max
}eConnectDirection;

typedef enum
{
    eConnectionPoint_None,
    eConnectionPoint_From,
    eConnectionPoint_To,
    eConnectionPoint_Max
}eConnectionPoint;

#endif // CONNECTIONDEFINITIONS_H
