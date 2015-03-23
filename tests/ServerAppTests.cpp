#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <Light.h>

using namespace std;

TEST(TestServerApp, MapConstrainSimple) {
    EXPECT_DOUBLE_EQ( home::mapConstrain(  0, 0, 1, 0, 100),   0); // low
    EXPECT_DOUBLE_EQ( home::mapConstrain(  1, 0, 1, 0, 100), 100); // high
    EXPECT_DOUBLE_EQ( home::mapConstrain(0.5, 0, 1, 0, 100),  50); // middle
    EXPECT_DOUBLE_EQ( home::mapConstrain(-20, 0, 1, 0, 100),   0); // way low
    EXPECT_DOUBLE_EQ( home::mapConstrain( 20, 0, 1, 0, 100), 100); // way high
}

TEST(TestServerApp, MapConstrainHarder) {
    EXPECT_DOUBLE_EQ( home::mapConstrain(-10, -10, 10, 500, 1000),  500); // low
    EXPECT_DOUBLE_EQ( home::mapConstrain( 10, -10, 10, 500, 1000), 1000); // high
    EXPECT_DOUBLE_EQ( home::mapConstrain(  0, -10, 10, 500, 1000),  750); // middle
    EXPECT_DOUBLE_EQ( home::mapConstrain(-20, -10, 10, 500, 1000),  500); // way low
    EXPECT_DOUBLE_EQ( home::mapConstrain( 20, -10, 10, 500, 1000), 1000); // way high
}

TEST(TestServerApp, MapConstrainAsymmetric) {
    EXPECT_DOUBLE_EQ( home::mapConstrain(-5.0, -5.0, 10.0, -99.2, 225.6), -99.2); // low
    EXPECT_DOUBLE_EQ( home::mapConstrain(10.0, -5.0, 10.0, -99.2, 225.6), 225.6); // high
    EXPECT_DOUBLE_EQ( home::mapConstrain( 2.5, -5.0, 10.0, -99.2, 225.6),  63.2); // middle
    EXPECT_DOUBLE_EQ( home::mapConstrain( -20, -5.0, 10.0, -99.2, 225.6), -99.2); // way low
    EXPECT_DOUBLE_EQ( home::mapConstrain(  20, -5.0, 10.0, -99.2, 225.6), 225.6); // way high
}

