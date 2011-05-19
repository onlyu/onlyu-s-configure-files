#include <gtest/gtest.h>
namespace{

class FooTest: public ::testing::Test{
protected:
	FooTest(){}
	~FooTest(){}
	virtual void SetUp(){
	  count = 1000000;
	}
	virtual void TearDown(){

	}	
  static int count;
};

TEST_F(FooTest, JustForTest){
	EXPECT_EQ(0,0);
	int a = 100;
	int b = 1000;
	ASSERT_EQ(a,b);
	EXPECT_EQ(100,100);
}

 TEST_F(FooTest, TestPerformace){
   int i=0;
   for(i=0;i<count;i++){
     EXPECT_EQ(i,i);
   } 
 }
 TEST_F(FooTest, TestLoop){
   int a=0;
   for(int i=0;i<count;i++){
     a++;
   }
   EXPECT_EQ(a, count);
 }
 TEST_F(FooTest, TestLoop2){
   int a=0;
   for(int i=0;i<count;i++){
     a = a+1;
     a = i*i;
     a = i*1.0/i;
   }
 }
};
int main(int argc,char **argv)
{
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
