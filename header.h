
#include <stdint.h>

#define CONTAINER_TREE	0
#define CONTAINER_HASH	1

struct Set {
	uint8_t type_of_container;
	size_t key_size;
	size_t data_size;
	
	void* structure_pointer;

	void (*set)(struct Set* S, const void* key, const void* data);
	void (*get)(struct Set* S, const void* key, void* buffer);
	uint8_t (*find)(struct Set* S, const void* key);
	void (*destroy)(struct Set* S);
	void (*delete)(struct Set* S, const void* key);
	void (*clear)(struct Set* S);
};

struct Set* SetTreeCreate(size_t key_size, size_t data_size);
//struct Set* SetHashCreate(size_t key_size, size_t data_size);



 
