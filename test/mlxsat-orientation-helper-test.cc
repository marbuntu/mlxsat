


#include <ns3/core-module.h>
#include <ns3/sat-isl-terminal.h>
#include <ns3/test.h>


NS_LOG_COMPONENT_DEFINE("OrientationHelperTest");


namespace ns3
{


class OrientationHelperTestCase : public TestCase
{

public: 
    OrientationHelperTestCase(std::string name, double minValue);


private:

    virtual void DoRun();
    virtual void DoTeardown();

};


class OrientationHelperTestSuite : public TestSuite
{
public:
    OrientationHelperTestSuite();

};


OrientationHelperTestCase::OrientationHelperTestCase(std::string name, double minValue)
: TestCase(name)
{
    NS_LOG_FUNCTION(this << name << minValue);
}


void OrientationHelperTestCase::DoRun()
{
    NS_LOG_FUNCTION(this);
}


void OrientationHelperTestCase::DoTeardown()
{
    NS_LOG_FUNCTION(this);
}





OrientationHelperTestSuite::OrientationHelperTestSuite()
: TestSuite("orientation-helper-test", UNIT)
{

    AddTestCase(
        new OrientationHelperTestCase("test-1", 0.0),
        TestCase::QUICK
    );

}

static OrientationHelperTestSuite g_OrientationHelperTestSuiteInstance;


}   /* namespace ns3 */