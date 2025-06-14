#include <TestConfigWidget.h>

#include <CommonGraphics/ConfigWidget.h>

QTEST_MAIN(TestConfigWidget)


void TestConfigWidget::open()
{
    ConfigWidget::getInstance();

    m_isFocusOnFirstZoneCalled = false;
    ConfigWidget::open(this);

    QVERIFY(m_isFocusOnFirstZoneCalled);

    ConfigWidget::deleteInstance();
}
