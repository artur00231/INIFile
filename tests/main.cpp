#include <iostream>

#include "INIFile.h"

#include "gtest/gtest.h"
#include "tests.h"
#include "parser_tests.h"

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
