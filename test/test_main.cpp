#include "test_main.hpp"

std::string mode;
Calculator calculator(mode == "1");

TEST(all_tests, sum) 
{ 
	double result = 5;
	calculator.Calculate ("2+3");
	ASSERT_TRUE(result == calculator.lastResult);
}

TEST(all_tests, mult) 
{ 
	double result = 5;
	calculator.Calculate("1*5");
	ASSERT_TRUE(result == calculator.lastResult);
}


TEST(all_tests, div) 
{ 
	double result = 5;
	calculator.Calculate("5/1");
	ASSERT_TRUE(result == calculator.lastResult);
}

TEST(all_tests, primer) 
{ 
	double result = 10;
	calculator.Calculate("5*0+3*3+1");
	ASSERT_TRUE(result == calculator.lastResult);
}

TEST(all_tests, Long_primer2)
{
	double result = 910.97222222222217;
	calculator.Calculate("10+(52-18/8*9)+4^7/18+(12-53)");
	ASSERT_TRUE(result == calculator.lastResult);
}

TEST(all_tests, Long_primer3)
{
	double result = 199313.80833333332;
	calculator.Calculate("2^2^3/15*(5+6^3^2)-15/8+6*9/8+222");
	ASSERT_TRUE(result == calculator.lastResult);
}

TEST(all_tests, by_zero) 
{ 
	double result = 0;
	calculator.Calculate("0*5");
	ASSERT_TRUE(result == calculator.lastResult);
}

TEST(all_tests, non_zero) 
{ 
	ASSERT_ANY_THROW(calculator.Calculate("5/0"););
}

TEST(all_tests, trigonom) 
{ 
	double result = sin(30);
	calculator.Calculate("sin(30)");
	ASSERT_TRUE(result == calculator.lastResult);
}

TEST(all_tests, sqrt) 
{
	double result = 12;
	calculator.Calculate("sqrt(144)");
	ASSERT_TRUE(result == calculator.lastResult);
}

TEST(all_tests, pow) 
{ 
	double result = 16;
	calculator.Calculate("4^2");
	ASSERT_TRUE(result == calculator.lastResult);
}

TEST(all_tests, invert) 
{ 
	double result = 1;
	calculator.Calculate("3/4+0.25");
	ASSERT_TRUE(result == calculator.lastResult);
}

TEST(all_tests, error) 
{ 
	ASSERT_ANY_THROW(calculator.Calculate("5+-3"););
}

TEST(all_tests, MW)
{
	calculator.Calculate("6+1");
	ASSERT_NO_THROW(calculator.Calculate("MW"););;
}

TEST(all_tests, MR)
{
	double result = 7;
	calculator.Calculate("MR");
	ASSERT_TRUE(result == calculator.lastResult);
}

int main(int argc, char **argv) 
{ 
	::testing::InitGoogleTest(&argc, argv); 
	return RUN_ALL_TESTS(); 
}