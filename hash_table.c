#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "header.h"

void	HashSetValue(struct Set* S, const void* key, const void* data);
uint8_t	HashGetValue(struct Set* S, const void* key, void* buffer);
uint8_t HashFindValue(struct Set* S, const void* key);
void*	HashInit(struct Set* S);
void	HashDestroy(struct Set* S);
void	HashDelete(struct Set* S, const void* key);
void	HashClear(struct Set* S);

struct HT* HTInit(size_t key_size, size_t data_size);
void HTSet(struct HT* H, const void* key, size_t key_size, const void* data, size_t data_size);
int HTGet(struct HT* H, const void* key, size_t key_size, void* buffer, size_t data_size);
int HTRemove(struct HT* H, const void* key, size_t key_size, size_t data_size);
uint8_t HTFind(struct HT* H, const void* key, size_t key_size, size_t data_size);
void HTClear(struct HT* H);
void HTDestroy(struct HT* H);


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

void HashDelete(struct Set* S, const void* key)
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

#define MAX_HASH UINT16_MAX * 256
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

	for (int i = 0; i < MAX_HASH; i++) {
		result->data[i] = (void*)calloc(key_size + data_size, ALLOCATION_STEP);
		if (result->data[i] == NULL)
			return NULL;
		result->allocated[i] = ALLOCATION_STEP;
	}
	return result;
}

void HTSet(struct HT* H, const void* key, size_t key_size, 
									const void* data, size_t data_size)
{
	uint32_t hash = FAQ6(key, key_size, MAX_HASH);
	for (int i = 0; i < H->used[hash]; i++) {
		void* pointer = (H->data[hash]) + (key_size + data_size) * i;
		if (memcmp(key, pointer, key_size) == 0) {
			memcpy(pointer + key_size, data, data_size);
			return;
		}
	}
	
	if (H->used[hash] < H->allocated[hash]) {
		size_t new_size = (H->allocated[hash] + ALLOCATION_STEP) * 
													(key_size + data_size);
		H->data[hash] = realloc (H->data[hash], new_size);
		H->allocated[hash] += ALLOCATION_STEP;
	}

	void* pointer = (H->data[hash]) + 
			(key_size + data_size) *  H->used[hash];

	memcpy(pointer, key, key_size);
	memcpy(pointer + key_size, data, data_size);
	H->used[hash]++;
	
	return;
}

int HTGet(struct HT* H, const void* key, size_t key_size, 
									void* buffer, size_t data_size)
{
	uint32_t hash = FAQ6(key, key_size, MAX_HASH);
	
	for (int i = 0; i < H->used[hash]; i++) {
		void* pointer = (H->data[hash]) + (key_size + data_size) * i;
		if (memcmp(key, pointer, key_size) == 0) {
			memcpy(buffer, pointer + key_size, data_size);
			return 1;
		}
	}
	return 0;
}

int HTRemove(struct HT* H, const void* key, size_t key_size, size_t data_size)
{
	uint32_t hash = FAQ6(key, key_size, MAX_HASH);

	for (int i = 0; i < H->used[hash]; i++) {
		void* pointer = (H->data[hash]) + (key_size + data_size) * i;
		if (memcmp(key, pointer, key_size) == 0) {
			if (i + 1 == H->used[hash]) 
				H->used[hash]--;
			else {
				void* pointer_final = (H->data[i]) + 
					(key_size + data_size) * (H->used[hash] - 1);
				memcpy(pointer, pointer_final, key_size + data_size);
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
	
	for (int i = 0; i < H->used[hash]; i++) {
		void* pointer = (H->data[hash]) + (key_size + data_size) * i;
		if (memcmp(key, pointer, key_size) == 0) 
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

/*
int main(int argc, char** argv, char** env)
{
	size_t size = sizeof(int);
	
	int a = 5, aa = 25;
	struct HT* H = HTInit(size, size);
	HTSet(H, &a, size, &aa, size);
	aa = 44;
	int status = HTGet(H, &a, size, &aa, size);
	printf("status = %d\n", status);
	printf("aa = %d\n", aa);
	
	HTClear(H);
	aa = 66;
	status = HTGet(H, &a, size, &aa, size);
	printf("status = %d\n");
	printf("aa = %d\n", aa);

	HTDestroy(H);
	return 0;
}
//*/






























