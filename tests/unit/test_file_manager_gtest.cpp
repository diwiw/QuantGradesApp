#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Simple checking test, whether GTest framework is working
TEST(FileManagerGTest, InfrastructureCheck) { ASSERT_TRUE(true); }

// Test checking definitions (UNIT_TEST from CMake)
TEST(FileManagerGTest, MacroDefinitionCheck)
{
#ifdef UNIT_TEST
    ASSERT_TRUE(true);
#else
    FAIL() << "UNIT_TEST macro not defined!";
#endif
}
