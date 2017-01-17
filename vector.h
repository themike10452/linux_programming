typedef struct dynamic_array {
	unsigned int size;
	unsigned int capacity;
	unsigned int _elemsize;
	int* _data;
} vec;

vec* vector(unsigned int elemsize, unsigned int capacity);

void* vector_get(vec* vec, unsigned int index);

int vector_getp(vec* vec, void* out, unsigned int index);

void vector_push(vec* vec, void* element, unsigned int elemsize);

void vector_free(vec* vec);