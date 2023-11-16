#include "lib.h"
#include <gtest/gtest.h>

TEST_F(TestSerialization, DateJson) {
    Date d1(1988, 8, 5);
    rapidjson::Document json = d1.toJSON();
    Date d2 = Date::fromJSON(json);
    ASSERT_EQ(, d2);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}