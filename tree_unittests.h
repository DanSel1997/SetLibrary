#include "header.h"


TEST(TREE, INSERT_AND_FIND)
{
	int a;
	long long int b;
	map_t M = MapTreeCreate(sizeof(int), sizeof(long long int));
	for (int i = 0; i < 10000; i++) {
		if (i % 2 == 0) {
			a = i;
			b = i * i * i;
			M->set(M, &a, &b);
		}
	}
	for (int i = 0; i < 10000; i++) {
		int status = M->find(M, &i);
		if (i % 2 == 0)
			ASSERT_EQ(status, 1);
		else
			ASSERT_EQ(status, 0);
	}
	M->destroy(M);
}

TEST(TREE, INSERT_AND_GET)
{
	int a;
	long long int b;
	map_t M = MapTreeCreate(sizeof(int), sizeof(long long int));
	for (int i = 0; i < 10000; i++) {
		if (i % 2 == 0) {
			a = i;
			b = i * i * i;
			M->set(M, &a, &b);
		}
	}
	for (int i = 0; i < 10000; i++) {
		long long int data;
		int status = M->get(M, &i, &data);
		if (i % 2 == 0) {
			ASSERT_EQ(status, 1);
			ASSERT_EQ(data, i * i * i);
		}
		else
			ASSERT_EQ(status, 0);
	}
	M->destroy(M);
}

TEST(TREE, INSERT_REWRITE_GET)
{
	int a;
	long long int b;
	map_t M = MapTreeCreate(sizeof(int), sizeof(long long int));
	for (int i = 0; i < 10000; i++) {
		a = i;
		b = i * i * i;
		M->set(M, &a, &b);
	}
	for (int i = 0; i < 10000; i += 2) {
		a = i;
		b = i * i;
		M->set(M, &a, &b);
	}

	for (int i = 0; i < 10000; i++) {
		long long int data;
		int status = M->get(M, &i, &data);
		if (i % 2 == 0) {
			ASSERT_EQ(status, 1);
			ASSERT_EQ(data, i * i);
		}
		else {
			ASSERT_EQ(status, 1);
			ASSERT_EQ(data, i * i * i);
		}
	}
	M->destroy(M);
}

TEST(TREE, INSERT_REMOVE_INSERT_FIND)
{
	int a;
	long long int b;
	map_t M = MapTreeCreate(sizeof(int), sizeof(long long int));
	for (int i = 0; i < 10000; i++) {
		a = i;
		b = i * i * i;
		M->set(M, &a, &b);
	}

	for (int i = 0; i < 10000; i += 2) {
		M->remove(M, &i);
	}

	for (int i = 0; i < 10000; i++) {
		int status = M->find(M, &i);
		if (i % 2 == 0)
			ASSERT_EQ(status, 0);
		else
			ASSERT_EQ(status, 1);
	}

	for (int i = 0; i < 10000; i += 4) {
		a = i;
		b = i * i * i;
		M->set(M, &a, &b);
	}

	for (int i = 0; i < 10000; i++) {
		int status = M->find(M, &i);
		if (i % 2 == 0 && i % 4 != 0)
			ASSERT_EQ(status, 0);
		else
			ASSERT_EQ(status, 1);
	}
	M->destroy(M);
}

TEST(TIME, TREE_INSERT_REWRITE_GET_1_000_000)
{
	int a;
	long long int b;
	map_t M = MapTreeCreate(sizeof(int), sizeof(long long int));
	for (int i = 0; i < 1000000; i++) {
		a = i;
		b = i * i * i;
		M->set(M, &a, &b);
	}
	for (int i = 0; i < 1000000; i += 2) {
		a = i;
		b = i * i;
		M->set(M, &a, &b);
	}

	for (int i = 0; i < 1000000; i++) {
		long long int data;
		int status = M->get(M, &i, &data);
		if (i % 2 == 0) {
			ASSERT_EQ(status, 1);
			ASSERT_EQ(data, i * i);
		}
		else {
			ASSERT_EQ(status, 1);
			ASSERT_EQ(data, i * i * i);
		}
	}
	M->destroy(M);
}

TEST(TIME, TREE_INSERT_REWRITE_GET_10_000_000)
{
	int a;
	long long int b;
	map_t M = MapTreeCreate(sizeof(int), sizeof(long long int));
	for (int i = 0; i < 10000000; i++) {
		a = i;
		b = i * i * i;
		M->set(M, &a, &b);
	}
	for (int i = 0; i < 10000000; i += 2) {
		a = i;
		b = i * i;
		M->set(M, &a, &b);
	}

	for (int i = 0; i < 10000000; i++) {
		long long int data;
		int status = M->get(M, &i, &data);
		if (i % 2 == 0) {
			ASSERT_EQ(status, 1);
			ASSERT_EQ(data, i * i);
		}
		else {
			ASSERT_EQ(status, 1);
			ASSERT_EQ(data, i * i * i);
		}
	}
	M->destroy(M);
}

