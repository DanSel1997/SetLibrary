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
};

struct Set* SetTreeCreate(size_t key_size, size_t data_size);
struct Set* SetHashCreate(size_t key_size, size_t data_size);

#ifdef __cplusplus
	}
#endif

 
