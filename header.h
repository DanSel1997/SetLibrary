#ifndef __LIBHEAADER_INCLUDE
#define __LIBHEAADER_INCLUDE

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

struct Set {
	size_t key_size;
	size_t data_size;
	
	void* structure_pointer;

	void (*set)(struct Set* S, const void* key, const void* data);
	uint8_t (*get)(struct Set* S, const void* key, void* buffer);
	uint8_t (*find)(struct Set* S, const void* key);
	void (*destroy)(struct Set* S);
	void (*remove)(struct Set* S, const void* key);
	void (*clear)(struct Set* S);
	void (*foreach)(struct Set* S, const void* arg, 
		void (*function)(const void* key, const void* data, const void* arg));
};

typedef struct Set* map_t;
map_t MapTreeCreate(size_t key_size, size_t data_size);
map_t MapHashCreate(size_t key_size, size_t data_size);


#ifdef __cplusplus
	}
#endif

#endif //__LIBHEAADER_INCLUDE
 
