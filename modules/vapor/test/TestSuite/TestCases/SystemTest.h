#ifndef _SYSTEM_TEST_H
#define _SYSTEM_TEST_H

#include <iostream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>


/*****************************************************************
 tests out the functionality expected of vpr::ReturnStatus
*******************************************************************/

namespace vprTest
{

class SystemTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SystemTest);
CPPUNIT_TEST( ShortConversions );
CPPUNIT_TEST( LongConversions );
CPPUNIT_TEST( LongLongConversions );
CPPUNIT_TEST( GetSetEnvTest );
CPPUNIT_TEST_SUITE_END();
public:   

   void ShortConversions();

   void LongConversions();

   void LongLongConversions();

   void GetSetEnvTest();
};

}

#endif

