#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "header.h"

#define MAX_HASH 256 * 256 * 256
#define ALLOCATION_STEP 2

struct HT {
	uint32_t*	allocated;
	uint32_t*	used;
	void**		data;
};
typedef struct HT* __hash_t;
/////////////////////////////////////////////////////////////////
#ifdef __cplusplus
	extern "C" {
#endif

void	HashSetValue(map_t S, const void* key, const void* data);
uint8_t	HashGetValue(map_t S, const void* key, void* buffer);
uint8_t HashFindValue(map_t S, const void* key);
void*	HashInit(map_t S);
void	HashDestroy(map_t S);
void	HashRemove(map_t S, const void* key);
void	HashClear(map_t S);
void	HashForeach(map_t S, const void* arg,
		void (*function)(const void* key, const void* data, const void* arg));



__hash_t HTInit(size_t key_size, size_t data_size);
void	HTSet(__hash_t H, const void* key, size_t key_size, 
										const void* data, size_t data_size);
int		HTGet(__hash_t H, const void* key, size_t key_size, 
										void* buffer, size_t data_size);
int		HTRemove(__hash_t H, const void* key, 
										size_t key_size, size_t data_size);
uint8_t	HTFind(__hash_t H, const void* key, 
										size_t key_size, size_t data_size);
void	HTClear(__hash_t H);
void	HTDestroy(__hash_t H);
void	HTForeach(__hash_t H, size_t key_size, size_t data_size, 
										const void* arg,
		void (*function)(const void* key, const void* data, const void* arg));


#ifdef __cplusplus
	}
#endif
///////////////////////////////////////////////////////////
void HashSetValue(map_t S, const void* key, const void* data)
{
	HTSet((__hash_t)(S->structure_pointer), key, S->key_size, data, S->data_size);
}
uint8_t	HashGetValue(map_t S, const void* key, void* buffer)
{
	return HTGet((__hash_t)(S->structure_pointer), key, S->key_size, buffer, S->data_size);
}

uint8_t HashFindValue(map_t S, const void* key)
{
	return HTFind((__hash_t)(S->structure_pointer), key, S->key_size, S->data_size);
}

void* HashInit(map_t S)
{
	__hash_t result = HTInit(S->key_size, S->data_size);
	return (void*) result;
}

void HashDestroy(map_t S)
{
	HTDestroy((__hash_t)(S->structure_pointer));
	free(S);
}

void HashRemove(map_t S, const void* key)
{
	HTRemove((__hash_t)(S->structure_pointer), key, S->key_size, S->data_size);
}

void HashClear(map_t S)
{
	HTClear((__hash_t)(S->structure_pointer));
}

uint32_t FAQ6(const void* data, size_t data_size, uint32_t hash_limit)
{
	if (hash_limit == 0) {
		printf("Hash_limit must be greater then zero");
		exit(-1);
	}
	uint8_t* iterator = (uint8_t*)data;
	uint32_t result = 0;
	for (size_t i = 0; i < data_size; i++, iterator++) {
		result += *iterator;
		result += result << 10;
		result ^= result >> 6;
	}
	result += result << 3;
	result ^= result >> 11;
	result += result << 15;
	return result % hash_limit;
}

void HashForeach(map_t S, const void* arg, 
		void (*function)(const void* key, const void* data, const void* arg))
{
	HTForeach((__hash_t)(S->structure_pointer), S->key_size, S->data_size, 
												arg, function);
}

////////////////////////////////////////////////////////////////////////
#define KEY_POINTER(H, line, index, key_size, data_size) \
	(((uint8_t*)H->data[line]) + ((key_size) + (data_size)) * (index))

#define DATA_POINTER(H, line, index, key_size, data_size) \
	((((uint8_t*)H->data[line]) + ((key_size) + (data_size)) * \
												(index)) + (key_size))


__hash_t HTInit(size_t key_size, size_t data_size)
{
	__hash_t result = (__hash_t) calloc (sizeof(struct HT), 1);
	if (result == NULL)
		return NULL;

	result->used = (uint32_t*) calloc (sizeof(uint32_t), MAX_HASH);
	result->allocated = (uint32_t*) calloc (sizeof(uint32_t), MAX_HASH);
	result->data = (void**) calloc (sizeof(void*), MAX_HASH);
	
	if (result->used == NULL || result->allocated == NULL 
												|| result->data == NULL) 
		return NULL;
	return result;
}

void HTSet(__hash_t H, const void* key, size_t key_size, 
									const void* data, size_t data_size)
{
	uint32_t hash = FAQ6(key, key_size, MAX_HASH);
	for (uint32_t i = 0; i < H->used[hash]; i++) {
		uint8_t* pointer = KEY_POINTER(H, hash, i, key_size, data_size);
		if (memcmp(key, (void*)pointer, key_size) == 0) {
			memcpy((void*)(pointer + key_size), data, data_size);
			return;
		}
	}
	if (H->used[hash] == H->allocated[hash]) {
		size_t new_size = (H->allocated[hash] + ALLOCATION_STEP) * 
													(key_size + data_size);
		H->data[hash] = realloc (H->data[hash], new_size);
		H->allocated[hash] += ALLOCATION_STEP;
	}
	uint8_t* pointer = ((uint8_t*)H->data[hash]) + 
			(key_size + data_size) *  H->used[hash];

	memcpy((void*)pointer, key, key_size);
	memcpy((void*)(pointer + key_size), data, data_size);
	H->used[hash]++;
	return;
}

int HTGet(__hash_t H, const void* key, size_t key_size, 
									void* buffer, size_t data_size)
{
	uint32_t hash = FAQ6(key, key_size, MAX_HASH);
	
	for (uint32_t i = 0; i < H->used[hash]; i++) {
		uint8_t* pointer = KEY_POINTER(H, hash, i, key_size, data_size);
		if (memcmp(key, (void*)pointer, key_size) == 0) {
			memcpy(buffer, (void*)(pointer + key_size), data_size);
			return 1;
		}
	}
	return 0;
}

int HTRemove(__hash_t H, const void* key, size_t key_size, size_t data_size)
{
	uint32_t hash = FAQ6(key, key_size, MAX_HASH);

	for (uint32_t i = 0; i < H->used[hash]; i++) {
		uint8_t* pointer = KEY_POINTER(H, hash, i, key_size, data_size);

		if (memcmp(key, (void*)pointer, key_size) == 0) {
			if (i + 1 == H->used[hash])
				H->used[hash]--;
			else {
				uint8_t* pointer_final = KEY_POINTER(H, hash, 
								H->used[hash] - 1, key_size, data_size);
				memcpy((void*)pointer, (void*)pointer_final, 
													key_size + data_size);
				H->used[hash]--;
			}
			return 1;
		}
	}
	return 0;
}

uint8_t HTFind(__hash_t H, const void* key, size_t key_size, size_t data_size)
{
	uint32_t hash = FAQ6(key, key_size, MAX_HASH);
	
	for (uint32_t i = 0; i < H->used[hash]; i++) {
		uint8_t* pointer = KEY_POINTER(H, hash, i, key_size, data_size);
		if (memcmp(key, (void*)pointer, key_size) == 0) 
			return 1;
	}
	return 0;
}

void HTClear(__hash_t H)
{
	for (int i = 0; i < MAX_HASH; i++) {
		H->used[i] = 0;
	}
}

void HTDestroy(__hash_t H)
{
	for (int i = 0; i < MAX_HASH; i++)
		free(H->data[i]);
	free(H->used);
	free(H->allocated);
}

void HTForeach(__hash_t H, size_t key_size, size_t data_size, const void* arg,
		void (*function)(const void* key, const void* data, const void* arg))
{
	for (uint32_t i = 0; i < MAX_HASH; i++)
	{
		for (uint32_t j = 0; j < H->used[i]; j++) {
			void* key = KEY_POINTER(H, i, j, key_size, data_size);
			void* data = (uint8_t*)key + key_size;
			function(key, data, arg);
		}
	}
	return;
}






























