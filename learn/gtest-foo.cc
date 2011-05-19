#include <gtest/gtest.h>
namespace{

class FooTest: public ::testing::Test{
protected:
	FooTest(){}
	~FooTest(){}
	virtual void SetUp(){

	}
	virtual void TearDown(){

	}	
};

TEST_F(FooTest, JustForTest){
	EXPECT_EQ(0,0);
	EXPECT_EQ(0,1);
}
};
int main(int argc,char **argv)
{
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
