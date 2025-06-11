#include <TestConfigWidget.h>

#include <CommonGraphics/ConfigWidget.h>

QTEST_MAIN(TestConfigWidget)

void TestConfigWidget::open()
{
    QVERIFY(true);

    ConfigWidget::getInstance();
}
