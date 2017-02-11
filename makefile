

all: test.c libset.a
	gcc-6 test.c libset.a -o test

libcore.o: libcore.c header.h
	gcc-6 libcore.c -c -o libcore.o

avl_tree.o: avl_tree.c header.h
	gcc-6 avl_tree.c -c -o avl_tree.o

hash_table.o: hash_table.c header.h
	gcc-6 hash_table.c -c -o hash_table.o

libset.a: libcore.o avl_tree.o hash_table.o
	ar -cr libset.a libcore.o avl_tree.o hash_table.o


