#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "header.h"

// Prototypes of main Tree functions
void	TreeSetValue(struct Set* S, const void* key, const void* data);
int		TreeGetValue(struct Set* S, const void* key, void* buffer);
uint8_t TreeFindValue(struct Set* S, const void* key);
void*	TreeInit();
void	TreeDestroy(struct Set* S);
void	TreeDelete(struct Set* S, const void* key);
void	TreeClear(struct Set* S);

// Prototypes of main Hash functions
//void	HashSetValue(struct Set* S, const void* key, const void* data);
//int		HashGetValue(struct Set* S, const void* key, void* buffer);
//uint8_t HashFindValue(struct Set* S, const void* key);
//void*	HashInit();
//void	HashDestroy(struct Set* S);
//void	HashDelete(struct Set* S, const void* key);
//void	HashClear(struct Set* S);

struct Set* SetTreeCreate(size_t key_size, size_t data_size)
{
	struct Set* result = (struct Set*) calloc (sizeof(struct Set), 1);

	result->key_size = key_size;
	result->data_size = data_size;
	result->type_of_container = CONTAINER_TREE;

	result->structure_pointer =  TreeInit();

	result->set = TreeSetValue;
	result->get = TreeGetValue;
	result->find = TreeFindValue;
	result->destroy = TreeDestroy;
	result->delete = TreeDelete;
	result->clear = TreeClear;
}

struct Set* SetHashCreate(size_t key_size, size_t data_size)
{
	struct Set* result = (struct Set*) calloc (sizeof(struct Set), 1);

	result->key_size = key_size;
	result->data_size = data_size;
	result->type_of_container = CONTAINER_HASH;

//	result->structure_pointer =  HashInit();

//	result->set = HashSetValue;
//	result->get = HashGetValue;
//	result->find = HashFindValue;
//	result->destroy = HashDestroy;
//	result->delete = HashDelete;
//	result->clear = HashClear;
}


