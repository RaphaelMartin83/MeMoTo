#include "SMDiagramScene.h"

static const unsigned int s_StateMachineSceneSize = 100000U; // Square
static const char* s_StateMachineString = "StateMachine";

SMDiagramScene::SMDiagramScene():
    m_ToolBox()
{
    this->setSceneRect(QRectF(0.0, 0.0, s_StateMachineSceneSize, s_StateMachineSceneSize));
}

I_ToolBox* SMDiagramScene::getToolBox()
{
    static bool ls_isToolBoxCreated = false;

    if( false == ls_isToolBoxCreated )
    {
        ls_isToolBoxCreated = true;
        m_ToolBox = new SMToolBoxScene();
        m_ToolBox->registerToolBoxListener(this);
    }

    return m_ToolBox;
}

QString SMDiagramScene::getDiagramString()
{
    return s_StateMachineString;
}

QString SMDiagramScene::getSerializableName() const
{
    return s_StateMachineString;
}

QPoint SMDiagramScene::getStartPosition() const
{
    // By now the middle but could be different
    if(this->getAllSelectables().size() == 0)
    {
        return QPoint(0, 0);
    }
    else
    {
        QPointF centerFloat = this->itemsBoundingRect().center();
        QPoint centerInt(centerFloat.x(), centerFloat.y());
        return centerInt;
    }
}

unsigned int SMDiagramScene::getDiagramMaxWidth() const
{
    return s_StateMachineSceneSize;
}
unsigned int SMDiagramScene::getDiagramMaxHeight() const
{
    return s_StateMachineSceneSize;
}
