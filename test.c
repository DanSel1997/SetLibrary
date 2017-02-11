#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "header.h"

int main(int argc, char** argv, char** env)
{
	struct Set* S = SetHashCreate(sizeof(int), sizeof(int));

	uint8_t set_and_get_test = 1;

	for (int i = 0; i < 1000; i++) {
		int a = i * 1007 % 999;
		int aa = a * a;
		S->set(S, &a, &aa);
	}
	for (int i = 0; i < 1000; i++) {
		int a = i * 1007 % 999;
		int aa;
		S->get(S, &a, &aa);
		if (aa != a * a)
			set_and_get_test = 0;
	}
	printf( (set_and_get_test) ? "main test OK\n" : "main test Failed\n" );
	
	S->clear(S);

	double time;

	time = clock();
	for (int i = 0; i < 100000; i++) {
		int a = i;
		int aa = i * i;
		S->set(S, &a, &aa);	
	}
	time = (clock() - time) / CLOCKS_PER_SEC;
	printf("Time for 100000 insertions: %lg sec\n", time);
	S->clear(S);

	time = clock();
	for (int i = 0; i < 1000000; i++) {
		int a = i;
		int aa = i * i;
		S->set(S, &a, &aa);	
	}
	time = (clock() - time) / CLOCKS_PER_SEC;
	printf("Time for 1000000 insertions: %lg sec\n", time);
	S->clear(S);

	time = clock();
	for (int i = 0; i < 10000000; i++) {
		int a = i;
		int aa = i * i;
		S->set(S, &a, &aa);	
	}
	time = (clock() - time) / CLOCKS_PER_SEC;
	printf("Time for 10000000 insertions: %lg sec\n", time);

	S->destroy(S);


	return 0;
}
