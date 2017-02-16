#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "gtest/gtest.h"
#include "header.h"

#include "tree_unittests.h"
#include "hash_unittests.h"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
