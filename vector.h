typedef struct dynamic_array {
	unsigned int size;
	unsigned int capacity;
	void** _data;
} vec;

vec* vector(unsigned int capacity);

void vector_resize(vec* vec, unsigned int capacity);

void* vector_get(vec* vec, unsigned int index);

void vector_push(vec* vec, void* element);

int vector_remove(vec* vec, void* element);

void vector_removeat(vec* vec, unsigned int index);

int vector_find(vec* vec, void* element);

void vector_free(vec* vec);