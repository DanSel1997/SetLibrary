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

int shared_timer_reset(struct timeval* td)
{
	if (gettimeofday(td, NULL) == -1) {
		PERROR("Reset time error");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int shared_timer_dump(struct timeval* td)
{
	struct timeval dump_time;
	if (gettimeofday(&dump_time, NULL) == -1) {
		PERROR("Dump time error");
		return EXIT_FAILURE;
	}

	dump_time.tv_sec -= td->tv_sec;
	dump_time.tv_usec -= td->tv_usec;
	if (dump_time.tv_usec < 0) {
		dump_time.tv_sec -= 1;
		dump_time.tv_usec += 1000000;
	}
	double double_time = (double)dump_time.tv_sec + (double)dump_time.tv_usec / 1000000;
	
	#if !defined(DEBUG)
		#define DEBUG
		#define LOCK_DEBUG
	#endif

	log("\x1b[32m %6.03lf \x1b[0m", double_time);

	#if defined(LOCK_DEBUG)
		#undef LOCK_DEBUG
		#undef DEBUG
	#endif

	return EXIT_SUCCESS;
}

void insert_int_time_test(struct Set* S, size_t count)
{
	S->clear(S);
	struct timeval tm;
	log("insertion time test started, size = %d", (int)count);
	shared_timer_reset(&tm);
	for (uint32_t i = 0; i < count; i++) {
		int key = rand();
		int data = rand();
		S->set(S, &key, &data);
	}
	shared_timer_dump(&tm);
}

uint8_t insert_and_find_int_test(struct Set* S)
{
	S->clear(S);
	log("insertion and find test started");
	
	const int TSize = 20;

	for (int i = 0; i < TSize; i += 2) {
		int key = i;
		int data = key * key;
		S->set(S, &key, &data);
	}

	for (int i = 0; i < TSize; i += 3) {
		int key = i;
		int data = key * key * key;
		S->set(S, &key, &data);
	}
	for (int i = 0; i < TSize; i += 5) {
		int key = i;
			S->remove(S, &key);
	}

	for (int i = 0; i < TSize; i++) {
		int key = i;
		int data;
		int status = S->get(S, &key, &data);
		if (i % 5 == 0 && status != 0) {
			PERROR("deleted value was found :: %d", i);
			return 0;
		}
		if (i % 3 == 0 && i % 5 != 0 && 
									(status == 0 || key * key * key != data)) {
			
			PERROR("rewrited data pair wasn't returned correctly :: %d", i);
			return 0;
		}
		if (i % 2 == 0 && i % 3 != 0 && i % 5 != 0 && 
									(status == 0 || key * key != data)) {

			PERROR("initially data pair wasn't returned correctly :: %d:%d", 
																key, data);
			return 0;
		}
		if (i % 2 != 0 && i % 3 != 0 && status != 0) {
			PERROR("unknown data pair was found %d:%d", key, data);
			return 0;
		}
	}
	log("all tests passed");
	return 1;
}

int main(int argc, char** argv, char** env)
{
	struct Set* S;
	if (argc > 1 && !strcmp(argv[1], "-h")) {
		log("hash table mode");
		S = SetHashCreate(sizeof(int), sizeof(int));
	}
	else {
		log("tree mode");
		S = SetTreeCreate(sizeof(int), sizeof(int));
	}
	
	insert_and_find_int_test(S);

	insert_int_time_test(S, 1000);
	insert_int_time_test(S, 1000000);
	//insert_int_time_test(S, 10000000);

	S->destroy(S);
	return 0;
}
