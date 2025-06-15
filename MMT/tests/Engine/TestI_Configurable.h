#include <QObject>
#include <QTest>

#include <tests/stubs/Engine/ConfigWidget.h>

#include <Engine/I_Configurable.h>

class FakeConfigurable: public I_Configurable
{
    friend class TestI_Configurable;

public:
    virtual void openConfiguration()
    {

    }
    virtual void applyConfiguration()
    {

    }
};

class TestI_Configurable: public QObject
{
    Q_OBJECT
private slots:
    void destructor();

public:

private:
    bool m_isFocusOnFirstZoneCalled = false;
    bool m_isOKPressedCalled = false;
};
