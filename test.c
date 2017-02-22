#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#include <getopt.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/stat.h>

#include <fcntl.h>

#define DEBUG
#include "header.h"
#include "debug.h"

void F(const void* key, const void* data, const void* arg)
{
	printf("%d : %.04lf\n", *(int*)key, *(double*)data);
	*(int*)arg += *(int*)key;
}

int main(int argc, char** argv, char** env)
{
	map_t S = MapHashCreate(sizeof(int), sizeof(double));

	for (int i = 0; i < 20; i++) {
		double d = sqrt(i);
		S->set(S, &i, &d);
	}
	int sum = 0;

	S->foreach(S, (const void*)&sum, F);
	S->destroy(S);
	printf("sum = %d\n", sum);
	return 0;
}
