#define _BSD_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdint.h>


void	TreeSetValue(struct Set* S, const void* key, const void* data);
int		TreeGetValue(struct Set* S, const void* key, void* buffer);
uint8_t TreeFindValue(struct Set* S, const void* key);
void*	TreeInit();
void	TreeDestroy(struct Set* S);

struct AVLNode {
	void* key;
	void* value;
	struct AVLNode* left;
	struct AVLNode* right;
	int16_t height;
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
	return (P->leff != NULL) ? AVLFindMin(P->left) : P;
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
			P->left = AVLRotateLe(P->left);
		return AVLRotateRight(P);
	}
	return P;
}

static struct AVLNode* AVLInsert(struct AVLNode* P, const void* key, size_t key_size, const void* data, size_t data_size)
{
	if (P == NULL) {
		struct AVLNode* newNode = (struct AVLNode*) calloc (sizeof(struct AVLNode), 1);

		newNode->key = calloc (key_size, 1);
		if (newNode->key == NULL)
			return NULL;
		newNode->key = memcpy(newNode->key, key, key_size);

		newNode->data = calloc (data_size, 1);
		if (newNode->data == NULL)
			return NULL:
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

struct AVLNode* AVLRemove(struct AVLNode* P, const void* key, size_t key_size)
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

		free(P->key); free(P->data); free(P);
		
		if (!R)
			return Q;
		struct AVLNode* Min = AVLFindMin(R);
		Min->right = AVLRemoveMin(R);
		Min->left = Q;
		return AVLBalance(Min);
	}
	return AVLBalance(P);
}

uint8_t AVLFind(struct AVLNode* P, const void* key, size_t key_size)
{
	if (P = NULL)
		return 0;
	
	int status = memcmp(P->key, key, key_size);
	if (status == 0)
		return 1;
	
}









