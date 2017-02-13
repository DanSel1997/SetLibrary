#define _BSD_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include "header.h"

void	TreeSetValue(struct Set* S, const void* key, const void* data);
uint8_t	TreeGetValue(struct Set* S, const void* key, void* buffer);
uint8_t TreeFindValue(struct Set* S, const void* key);
void*	TreeInit();
void	TreeDestroy(struct Set* S);
void	TreeDelete(struct Set* S, const void* key);
void	TreeClear(struct Set* S);

struct AVLNode {
	void* key;
	void* data;
	struct AVLNode* left;
	struct AVLNode* right;
	int16_t height;
};

static int16_t			AVLHeight(struct AVLNode* P);
static int16_t			AVLBalanceFactor(struct AVLNode* P);
static struct			AVLNode* AVLFindMin(struct AVLNode* P);
static struct			AVLNode* AVLRemoveMin(struct AVLNode* P);
void					AVLFixHeight(struct AVLNode* P);
static struct AVLNode*	AVLRotateRight(struct AVLNode* P);
static struct AVLNode*	AVLRotateLeft(struct AVLNode* P);
static struct AVLNode*	AVLBalance(struct AVLNode* P);

static struct AVLNode*	AVLInsert(struct AVLNode* P, const void* key, size_t key_size, const void* data, size_t data_size);
static struct AVLNode*	AVLRemove(struct AVLNode* P, const void* key, size_t key_size);
static struct AVLNode*	AVLFind(struct AVLNode* P, const void* key, size_t key_size);
static void				AVLDestroy(struct AVLNode* P);		

void TreeSetValue(struct Set* S, const void* key, const void* data) 
{
	S->structure_pointer = (void*) AVLInsert ((struct AVLNode*)S->structure_pointer, key, S->key_size, data, S->data_size);
}

uint8_t	TreeGetValue(struct Set* S, const void* key, void* buffer)
{
	struct AVLNode* R = AVLFind((struct AVLNode*)S->structure_pointer, key, S->key_size);
	if (R == NULL)
		return 0;

	memcpy(buffer, R->data, S->data_size);
	return 1;
}

uint8_t TreeFindValue(struct Set* S, const void* key) 
{
	return (AVLFind((struct AVLNode*)S->structure_pointer, key, S->key_size) == NULL) ? 0 : 1;
}

void* TreeInit()
{
	return NULL;
}

void TreeDestroy(struct Set* S)
{
	AVLDestroy((struct AVLNode*)S->structure_pointer);
	free(S);
}

void TreeDelete(struct Set* S, const void* key)
{
	S->structure_pointer = (void*)AVLRemove((struct AVLNode*)S->structure_pointer, key, S->key_size);
}

void TreeClear(struct Set* S)
{
	AVLDestroy((struct AVLNode*)S->structure_pointer);
	S->structure_pointer = NULL;
}


static int16_t AVLHeight(struct AVLNode* P) 
{
	return P ? P->height : 0;
}

static int16_t AVLBalanceFactor(struct AVLNode* P) 
{
	return AVLHeight(P->right) - AVLHeight(P->left);
}

static struct AVLNode* AVLFindMin(struct AVLNode* P) 
{
	return (P->left != NULL) ? AVLFindMin(P->left) : P;
}

static struct AVLNode* AVLRemoveMin(struct AVLNode* P) 
{
	if (P->left == NULL)
		return P->right;
	P->left = AVLRemoveMin(P->left);
	return AVLBalance(P);
}

void AVLFixHeight(struct AVLNode* P)
{
	int16_t heightLeft = AVLHeight(P->left);
	int16_t heightRight = AVLHeight(P->right);
	P->height = ((heightLeft > heightRight) ? heightLeft : heightRight) + 1;
}

static struct AVLNode* AVLRotateRight(struct AVLNode* P)
{
	struct AVLNode* Q = P->left;
	P->left = Q->right;
	Q->right = P;
	AVLFixHeight(P);
	AVLFixHeight(Q);
	return Q;
}

static struct AVLNode* AVLRotateLeft(struct AVLNode* P)
{
	struct AVLNode* Q = P->right;
	P->right = Q->left;
	Q->left = P;
	AVLFixHeight(P);
	AVLFixHeight(Q);
	return Q;
}

static struct AVLNode* AVLBalance(struct AVLNode* P)
{
	AVLFixHeight(P);
	int16_t balanceFactor = AVLBalanceFactor(P);
	if (balanceFactor == 2) {
		if (AVLBalanceFactor(P->right) < 0)
			P->right = AVLRotateRight(P->right);
		return AVLRotateLeft(P);
	}
	if (balanceFactor == -2) {
		if (AVLBalanceFactor(P->left) > 0)
			P->left = AVLRotateLeft(P->left);
		return AVLRotateRight(P);
	}
	return P;
}

static struct AVLNode* AVLInsert(struct AVLNode* P, const void* key, size_t key_size, const void* data, size_t data_size)
{
	if (P == NULL) {
		struct AVLNode* newNode = (struct AVLNode*) calloc (sizeof(struct AVLNode), 1);

		newNode->key = calloc (key_size + data_size, 1);
		if (newNode->key == NULL)
			return NULL;
		newNode->key = memcpy(newNode->key, key, key_size);

		newNode->data = newNode->key + key_size;
		newNode->data = memcpy(newNode->data, data, data_size);
		return newNode;
	}

	int status = memcmp(P->key, key, key_size);
	if (status == 0) {
		P->data = memcpy(P->data, data, data_size);
		return P;
	}
	if (status < 0) {
		P->left = AVLInsert(P->left, key, key_size, data, data_size);
	}
	if (status > 0) {
		P->right = AVLInsert(P->right, key, key_size, data, data_size);
	}
	return AVLBalance(P);
}

static struct AVLNode* AVLRemove(struct AVLNode* P, const void* key, size_t key_size)
{
	if (!P)
		return NULL;
	
	int status = memcmp(P->key, key, key_size);
	if (status < 0) {
		P->left = AVLRemove(P->left, key, key_size);
	}
	else if (status > 0) {
		P->right = AVLRemove(P->right, key, key_size);
	}
	else {
		struct AVLNode* Q = P->left;
		struct AVLNode* R = P->right;

		free(P->key); 
		free(P);
		
		if (!R)
			return Q;
		struct AVLNode* Min = AVLFindMin(R);
		Min->right = AVLRemoveMin(R);
		Min->left = Q;
		return AVLBalance(Min);
	}
	return AVLBalance(P);
}

static struct AVLNode* AVLFind(struct AVLNode* P, const void* key, size_t key_size)
{
	if (P == NULL)
		return NULL;

	int status = memcmp(P->key, key, key_size);
	if (status == 0)
		return P;
	if (status < 0)
		return AVLFind(P->left, key, key_size);
	if (status > 0)
		return AVLFind(P->right, key, key_size);

	return NULL;	
}

static void AVLDBGPrint(struct AVLNode* P, int tab)
{
	if (P != NULL) {
		AVLDBGPrint(P->left, tab + 1);
		for (int i = 0; i < tab; i++)
			printf("|    ");
		printf("%d -> %d\n", *(int*)P->key, *(int*)P->data);
		AVLDBGPrint(P->right, tab + 1);
	}
}

static void	AVLDestroy(struct AVLNode* P)
{
	if (P == NULL)
		return;
	
	free(P->key);

	AVLDestroy(P->left);
	AVLDestroy(P->right);
	free(P);
}
/*
int main(int argc, char** argv, char** env)
{
	size_t key_size = sizeof(int);
	size_t data_size = sizeof(int);

	struct AVLNode* S = NULL;

	int a = 4;
	int b = 6;
	int c = 8;
	
	int aa = 44;
	int bb = 21;
	int cc = 43;

	int d;
	int dd;
	
	for (int j = 0; j < 10; j++) {
		a = rand() % 10;
		aa = a * a;
		S = AVLInsert(S, &a, key_size, &aa, data_size);
		AVLDBGPrint(S, 0);
		printf("-------------------------------\n");
	}
	for (int j = 0; j < 10; j++) {
		a = rand() % 10;
		struct AVLNode* F = AVLFind(S, &a, key_size);
		printf("a = %d, F = %p\n", a, F);
		if (F != NULL)
			printf("aa = %d\n", *(int*)F->data);
	}

}
//*/
