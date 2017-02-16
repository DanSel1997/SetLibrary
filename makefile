OPT = -Wall -O3

all: test.c libmap.a
	g++-6 $(OPT) test.c libmap.a -o test

gtest: gtest.cpp header.h libmap.a tree_unittests.h hash_unittests.h
	g++-6 $(OPT) gtest.cpp libmap.a -pthread -lgtest -o gtest 

libcore.o: libcore.c header.h
	g++-6 $(OPT) libcore.c -c -o libcore.o

avl_tree.o: avl_tree.c header.h
	g++-6 $(OPT) avl_tree.c -c -o avl_tree.o

hash_table.o: hash_table.c header.h
	g++-6 $(OPT) hash_table.c -c -o hash_table.o

libmap.a: libcore.o avl_tree.o hash_table.o
	ar -cr libmap.a libcore.o avl_tree.o hash_table.o


