OPT = -Wall -O3

all: test.c libset.a
	g++-6 $(OPT) test.c libset.a -o test

libcore.o: libcore.c header.h
	g++-6 $(OPT) libcore.c -c -o libcore.o

avl_tree.o: avl_tree.c header.h
	g++-6 $(OPT) avl_tree.c -c -o avl_tree.o

hash_table.o: hash_table.c header.h
	g++-6 $(OPT) hash_table.c -c -o hash_table.o

libset.a: libcore.o avl_tree.o hash_table.o
	ar -cr libset.a libcore.o avl_tree.o hash_table.o


