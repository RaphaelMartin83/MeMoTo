#include "ClassDiagramScene.h"

static const unsigned int s_ClassSceneSize = 100000U; // Square
static const char* s_ClassDiagramName = "ClassDiagram";

ClassDiagramScene::ClassDiagramScene():
    m_ToolBox(nullptr)
{
    this->setSceneRect(QRectF(0.0, 0.0, s_ClassSceneSize, s_ClassSceneSize));
}

// I_DiagramContainer
I_ToolBox* ClassDiagramScene::getToolBox()
{
    static bool ls_isToolBoxCreated = false;

    if( false == ls_isToolBoxCreated )
    {
        ls_isToolBoxCreated = true;
        m_ToolBox = new ClassToolBoxScene();
        m_ToolBox->registerToolBoxListener(this);
    }

    return m_ToolBox;
}

QString ClassDiagramScene::getDiagramString()
{
    return s_ClassDiagramName;
}

QString ClassDiagramScene::getSerializableName() const
{
    return s_ClassDiagramName;
}

QPoint ClassDiagramScene::getStartPosition() const
{
    // By now the middle but could be different
    if(this->getAllSelectables().size() == 0)
    {
        return QPoint(s_ClassSceneSize/2U, s_ClassSceneSize/2U);
    }
    else
    {
        QPointF centerFloat = this->itemsBoundingRect().center();
        QPoint centerInt(centerFloat.x(), centerFloat.y());
        return centerInt;
    }
}

unsigned int ClassDiagramScene::getDiagramMaxWidth() const
{
    return s_ClassSceneSize;
}
unsigned int ClassDiagramScene::getDiagramMaxHeight() const
{
    return s_ClassSceneSize;
}
