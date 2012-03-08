#include <gtest/gtest.h>

extern "C" {
extern int edit_distance(const char *word1, const char *word2);
}

TEST(edit_distance_test, test_distance) {
	EXPECT_EQ(0, edit_distance("test", "test"));
	EXPECT_EQ(7, edit_distance("appropriate meaning", "approximate matching"));
}

TEST(edit_distance_test, performance) {
	const char *str1 = "this is a test string1";
	const char *str2 = "this is a test string2";
	for (int i=0; i<1000; i++) {
		EXPECT_EQ(1, edit_distance(str1, str2));
	}
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
