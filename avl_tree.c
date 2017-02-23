#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include "header.h"

#ifdef __cplusplus
	extern "C" {
#endif
void	TreeSetValue(struct Set* S, const void* key, const void* data);
uint8_t	TreeGetValue(struct Set* S, const void* key, void* buffer);
uint8_t TreeFindValue(struct Set* S, const void* key);
void*	TreeInit();
void	TreeDestroy(struct Set* S);
void	TreeRemove(struct Set* S, const void* key);
void	TreeClear(struct Set* S);
void	TreeForeach(struct Set* S, const void* arg,
		void (*function)(const void* key, const void* data, const void* arg));

struct AVLNode {
	void* key;
	void* data;
	struct AVLNode* left;
	struct AVLNode* right;
	int16_t height;
};

typedef struct AVLNode* __node_t;

static int16_t			AVLHeight(__node_t P);
static int16_t			AVLBalanceFactor(__node_t P);
static struct			AVLNode* AVLFindMin(__node_t P);
static struct			AVLNode* AVLRemoveMin(__node_t P);
void					AVLFixHeight(__node_t P);
static __node_t	AVLRotateRight(__node_t P);
static __node_t	AVLRotateLeft(__node_t P);
static __node_t	AVLBalance(__node_t P);

static __node_t	AVLInsert(__node_t P, const void* key, 
		size_t key_size, const void* data, size_t data_size);
static __node_t	AVLRemove(__node_t P, const void* key, size_t key_size);
static __node_t	AVLFind(__node_t P, const void* key, size_t key_size);
static void				AVLDestroy(__node_t P);		
void		  AVLForeach(__node_t P, const void* arg,
		void (*function)(const void* key, const void* data, const void* arg));

#ifdef __cplusplus
	}
#endif

void TreeSetValue(struct Set* S, const void* key, const void* data) 
{
	S->structure_pointer = (void*) AVLInsert ((__node_t)S->structure_pointer, 
										key, S->key_size, data, S->data_size);
}

uint8_t	TreeGetValue(struct Set* S, const void* key, void* buffer)
{
	__node_t R = AVLFind((__node_t)S->structure_pointer, key, S->key_size);
	if (R == NULL)
		return 0;

	memcpy(buffer, R->data, S->data_size);
	return 1;
}

uint8_t TreeFindValue(struct Set* S, const void* key) 
{
	return (AVLFind((__node_t)S->structure_pointer, key, S->key_size)
														== NULL) ? 0 : 1;
}

void* TreeInit()
{
	return NULL;
}

void TreeDestroy(struct Set* S)
{
	AVLDestroy((__node_t)S->structure_pointer);
	free(S);
}

void TreeRemove(struct Set* S, const void* key)
{
	S->structure_pointer = (void*)AVLRemove((__node_t)S->structure_pointer, 
											key, S->key_size);
}

void TreeClear(struct Set* S)
{
	AVLDestroy((__node_t)S->structure_pointer);
	S->structure_pointer = NULL;
}
void	TreeForeach(struct Set* S, const void* arg,
		void (*function)(const void* key, const void* data, const void* arg))
{
	AVLForeach((__node_t)(S->structure_pointer), arg, function);
}

////////////////////////////////////////////////////////////////////////////
static int16_t AVLHeight(__node_t P) 
{
	return P ? P->height : 0;
}

static int16_t AVLBalanceFactor(__node_t P) 
{
	return AVLHeight(P->right) - AVLHeight(P->left);
}

static __node_t AVLFindMin(__node_t P) 
{
	return (P->left != NULL) ? AVLFindMin(P->left) : P;
}

static __node_t AVLRemoveMin(__node_t P) 
{
	if (P->left == NULL)
		return P->right;
	P->left = AVLRemoveMin(P->left);
	return AVLBalance(P);
}

void AVLFixHeight(__node_t P)
{
	int16_t heightLeft = AVLHeight(P->left);
	int16_t heightRight = AVLHeight(P->right);
	P->height = ((heightLeft > heightRight) ? heightLeft : heightRight) + 1;
}

static __node_t AVLRotateRight(__node_t P)
{
	__node_t Q = P->left;
	P->left = Q->right;
	Q->right = P;
	AVLFixHeight(P);
	AVLFixHeight(Q);
	return Q;
}

static __node_t AVLRotateLeft(__node_t P)
{
	__node_t Q = P->right;
	P->right = Q->left;
	Q->left = P;
	AVLFixHeight(P);
	AVLFixHeight(Q);
	return Q;
}

static __node_t AVLBalance(__node_t P)
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

static __node_t AVLInsert(__node_t P, const void* key, 
						size_t key_size, const void* data, size_t data_size)
{
	if (P == NULL) {
		__node_t newNode = (__node_t) calloc (sizeof(struct AVLNode), 1);

		newNode->key = calloc (key_size + data_size, 1);
		if (newNode->key == NULL)
			return NULL;
		newNode->key = memcpy(newNode->key, key, key_size);

		newNode->data = (void*)((uint8_t*)newNode->key + key_size);
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

static __node_t AVLRemove(__node_t P, const void* key, size_t key_size)
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
		__node_t Q = P->left;
		__node_t R = P->right;

		free(P->key); 
		free(P);
		
		if (!R)
			return Q;
		__node_t Min = AVLFindMin(R);
		Min->right = AVLRemoveMin(R);
		Min->left = Q;
		return AVLBalance(Min);
	}
	return AVLBalance(P);
}

static __node_t AVLFind(__node_t P, const void* key, size_t key_size)
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
/*
static void AVLDBGPrint(__node_t P, int tab)
{
	if (P != NULL) {
		AVLDBGPrint(P->left, tab + 1);
		for (int i = 0; i < tab; i++)
			printf("|    ");
		printf("%d -> %d\n", *(int*)P->key, *(int*)P->data);
		AVLDBGPrint(P->right, tab + 1);
	}
} 
//*/

static void	AVLDestroy(__node_t P)
{
	if (P == NULL)
		return;
	
	free(P->key);

	AVLDestroy(P->left);
	AVLDestroy(P->right);
	free(P);
}
void AVLForeach(__node_t P, const void* arg,
		void (*function)(const void* key, const void* data, const void* arg))
{
	if (P == NULL)
		return;
	AVLForeach(P->right, arg, function);		
	function(P->key, P->data, arg);
	AVLForeach(P->left, arg, function);	
	return;
}
