#include <QObject>
#include <QTest>

#include <Engine/I_ConfigurationContent.h>

class TestConfigWidget: public I_ConfigurationContent
{
    Q_OBJECT
private slots:
    void open();

public:
    virtual void setFocusOnFirstZone()
    {
        m_isFocusOnFirstZoneCalled = true;
    }
    virtual void OKPressed()
    {
        m_isOKPressedCalled = true;
    }

private:
    bool m_isFocusOnFirstZoneCalled = false;
    bool m_isOKPressedCalled = false;
};
