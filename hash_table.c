#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "header.h"

#ifdef __cplusplus
	extern "C" {
#endif

void	HashSetValue(struct Set* S, const void* key, const void* data);
uint8_t	HashGetValue(struct Set* S, const void* key, void* buffer);
uint8_t HashFindValue(struct Set* S, const void* key);
void*	HashInit(struct Set* S);
void	HashDestroy(struct Set* S);
void	HashRemove(struct Set* S, const void* key);
void	HashClear(struct Set* S);
void	HashForeach(struct Set* S, const void* arg,
		void (*function)(const void* key, const void* data, const void* arg));

struct HT* HTInit(size_t key_size, size_t data_size);
void HTSet(struct HT* H, const void* key, size_t key_size, const void* data, size_t data_size);
int HTGet(struct HT* H, const void* key, size_t key_size, void* buffer, size_t data_size);
int HTRemove(struct HT* H, const void* key, size_t key_size, size_t data_size);
uint8_t HTFind(struct HT* H, const void* key, size_t key_size, size_t data_size);
void HTClear(struct HT* H);
void HTDestroy(struct HT* H);
void HTForeach(struct HT* H, size_t key_size, size_t data_size, const void* arg,
		void (*function)(const void* key, const void* data, const void* arg));


#ifdef __cplusplus
	}
#endif

// HT
void HashSetValue(struct Set* S, const void* key, const void* data)
{
	HTSet((struct HT*)(S->structure_pointer), 
									key, S->key_size, data, S->data_size);
}
uint8_t	HashGetValue(struct Set* S, const void* key, void* buffer)
{
	return HTGet((struct HT*)(S->structure_pointer), 
									key, S->key_size, buffer, S->data_size);
}

uint8_t HashFindValue(struct Set* S, const void* key)
{
	return HTFind((struct HT*)(S->structure_pointer), 
									key, S->key_size, S->data_size);
}

void* HashInit(struct Set* S)
{
	struct HT* result = HTInit(S->key_size, S->data_size);
	return (void*) result;
}

void HashDestroy(struct Set* S)
{
	HTDestroy((struct HT*)(S->structure_pointer));
	free(S);
}

void HashRemove(struct Set* S, const void* key)
{
	HTRemove((struct HT*)(S->structure_pointer), key, S->key_size, S->data_size);
}

void HashClear(struct Set* S)
{
	HTClear((struct HT*)(S->structure_pointer));
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
void	HashForeach(struct Set* S, const void* arg,
		void (*function)(const void* key, const void* data, const void* arg))
{
	HTForeach((struct HT*)(S->structure_pointer), S->key_size, S->data_size, 
														arg, function);
}

////////////////////////////////////////////////////////////////////////
#define MAX_HASH 256 * 256 * 256
#define ALLOCATION_STEP 2

struct HT {
	uint32_t*	allocated;
	uint32_t*	used;
	void**		data;
};

struct HT* HTInit(size_t key_size, size_t data_size)
{
	struct HT* result = (struct HT*) calloc (sizeof(struct HT), 1);
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

void HTSet(struct HT* H, const void* key, size_t key_size, 
									const void* data, size_t data_size)
{
	uint32_t hash = FAQ6(key, key_size, MAX_HASH);
	for (uint32_t i = 0; i < H->used[hash]; i++) {
		uint8_t* pointer = ((uint8_t*)H->data[hash]) + (key_size + data_size) * i;
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

int HTGet(struct HT* H, const void* key, size_t key_size, 
									void* buffer, size_t data_size)
{
	uint32_t hash = FAQ6(key, key_size, MAX_HASH);
	
	for (uint32_t i = 0; i < H->used[hash]; i++) {
		uint8_t* pointer = ((uint8_t*)H->data[hash]) + (key_size + data_size) * i;
		if (memcmp(key, (void*)pointer, key_size) == 0) {
			memcpy(buffer, (void*)(pointer + key_size), data_size);
			return 1;
		}
	}
	return 0;
}

int HTRemove(struct HT* H, const void* key, size_t key_size, size_t data_size)
{
	uint32_t hash = FAQ6(key, key_size, MAX_HASH);

	for (uint32_t i = 0; i < H->used[hash]; i++) {
		uint8_t* pointer = ((uint8_t*)H->data[hash]) + (key_size + data_size) * i;
		if (memcmp(key, (void*)pointer, key_size) == 0) {
			if (i + 1 == H->used[hash])
				H->used[hash]--;
			else {
				uint8_t* pointer_final = ((uint8_t*)H->data[hash]) + 
					(key_size + data_size) * (H->used[hash] - 1);
				memcpy((void*)pointer, (void*)pointer_final, key_size + data_size);
				H->used[hash]--;
			}
			return 1;
		}
	}
	return 0;
}

uint8_t HTFind(struct HT* H, const void* key, size_t key_size, size_t data_size)
{
	uint32_t hash = FAQ6(key, key_size, MAX_HASH);
	
	for (uint32_t i = 0; i < H->used[hash]; i++) {
		uint8_t* pointer = ((uint8_t*)H->data[hash]) + (key_size + data_size) * i;
		if (memcmp(key, (void*)pointer, key_size) == 0) 
			return 1;
	}
	return 0;
}

void HTClear(struct HT* H)
{
	for (int i = 0; i < MAX_HASH; i++) {
		H->used[i] = 0;
	}
}

void HTDestroy(struct HT* H)
{
	for (int i = 0; i < MAX_HASH; i++)
		free(H->data[i]);
	free(H->used);
	free(H->allocated);
}

void HTForeach(struct HT* H, size_t key_size, size_t data_size, const void* arg,
		void (*function)(const void* key, const void* data, const void* arg))
{
	for (uint32_t i = 0; i < MAX_HASH; i++)
	{
		for (uint32_t j = 0; j < H->used[i]; j++) { 
			void* key = ((uint8_t*)H->data[i]) + 
					(key_size + data_size) * j;
			void* data = (uint8_t*)key + key_size;
			function(key, data, arg);
		}
	}
	return;
}






























