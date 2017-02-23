CCOMPILER = gcc-6
CPPCOMPLIER = g++-6
LINKER = g++-6
LINKER_OPTIONS = -lgtest
OPT = -O3 -Wall -Wpedantic -Werror

all: test.c libmap.a
	 $(CPPCOMPLIER) $(OPT) test.c libmap.a -o test

gtest: gtest.o libmap.a tree_unittests.o hash_unittests.o
	$(LINKER) gtest.o tree_unittests.o hash_unittests.o libmap.a $(LINKER_OPTIONS) -o gtest 


gtest.o: gtest.cpp header.h
	$(CPPCOMPLIER) $(OPT) gtest.cpp -c -o gtest.o

tree_unittests.o: tree_unittests.c
	$(CPPCOMPLIER) $(OPT) tree_unittests.c -c -o tree_unittests.o

hash_unittests.o: hash_unittests.c
	$(CPPCOMPLIER) $(OPT) hash_unittests.c -c -o hash_unittests.o



libcore.o: libcore.c header.h
	$(CCOMPILER) $(OPT) libcore.c -c -o libcore.o
ee.o: avl_tree.c header.h
	$(CCOMPILER) g++-6 $(OPT) avl_tree.c -c -o avl_tree.o

hash_table.o: hash_table.c header.h
	$(CCOMPILER) $(OPT) hash_table.c -c -o hash_table.o

libmap.a: libcore.o avl_tree.o hash_table.o
	ar -cr libmap.a libcore.o avl_tree.o hash_table.o


