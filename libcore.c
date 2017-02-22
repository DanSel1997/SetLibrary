#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "header.h"

#ifdef __cplusplus
	extern "C" {
#endif

// Prototypes of main Tree functions
void	TreeSetValue(struct Set* S, const void* key, const void* data);
uint8_t	TreeGetValue(struct Set* S, const void* key, void* buffer);
uint8_t TreeFindValue(struct Set* S, const void* key);
void*	TreeInit();
void	TreeDestroy(struct Set* S);
void	TreeRemove(struct Set* S, const void* key);
void	TreeClear(struct Set* S);
void	TreeForeach(struct Set* S, const void* arg, 
		void (*function)(const void* key, const void* data, const void* arg));

// Prototypes of main Hash functions
void	HashSetValue(struct Set* S, const void* key, const void* data);
uint8_t	HashGetValue(struct Set* S, const void* key, void* buffer);
uint8_t HashFindValue(struct Set* S, const void* key);
void*	HashInit(struct Set* S);
void	HashDestroy(struct Set* S);
void	HashRemove(struct Set* S, const void* key);
void	HashClear(struct Set* S);
void	HashForeach(struct Set* S, const void* arg,
		void (*function)(const void* key, const void* data, const void* arg));

#ifdef __cplusplus
	}
#endif

struct Set* MapTreeCreate(size_t key_size, size_t data_size)
{
	struct Set* result = (struct Set*) calloc (sizeof(struct Set), 1);

	result->key_size = key_size;
	result->data_size = data_size;
	result->structure_pointer =  TreeInit();

	result->set = TreeSetValue;
	result->get = TreeGetValue;
	result->find = TreeFindValue;
	result->destroy = TreeDestroy;
	result->remove = TreeRemove;
	result->clear = TreeClear;
	result->foreach = TreeForeach;

	return result;
}

struct Set* MapHashCreate(size_t key_size, size_t data_size)
{
	struct Set* result = (struct Set*) calloc (sizeof(struct Set), 1);

	result->key_size = key_size;
	result->data_size = data_size;
	result->structure_pointer =  HashInit(result);

	result->set = HashSetValue;
	result->get = HashGetValue;
	result->find = HashFindValue;
	result->destroy = HashDestroy;
	result->remove = HashRemove;
	result->clear = HashClear;
	result->foreach = HashForeach;

	return result;
}

