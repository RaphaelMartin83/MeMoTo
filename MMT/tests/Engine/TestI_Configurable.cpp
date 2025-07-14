#include <TestI_Configurable.h>

QTEST_MAIN(TestI_Configurable)


void TestI_Configurable::destructor()
{
    QVERIFY(false == ConfigWidget_isCloseCalled);

    FakeConfigurable* fakeConfigurable = new FakeConfigurable();
    delete fakeConfigurable;

    QVERIFY(true == ConfigWidget_isCloseCalled);
}
