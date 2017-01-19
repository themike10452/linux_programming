#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vector.h"

vec* vector(unsigned int capacity)
{
	vec* vec = malloc(sizeof(vec));
	if (!vec) return NULL;

	vec->_data = malloc(sizeof(void*) * capacity);
	if (!vec->_data) return NULL;

	vec->size = 0;
	vec->capacity = capacity;

	return vec;
}

void vector_resize(vec* vec, unsigned int capacity)
{
	void** data = realloc(vec->_data, sizeof(void*) * capacity);
	if (data)
	{
		vec->_data = data;
		vec->capacity = capacity;
	}
}

void* vector_get(vec* vec, unsigned int index)
{
	assert(index < vec->size);
	return vec->_data[index];
}

void vector_push(vec* vec, void* element)
{
	if (vec->size == vec->capacity)
		vector_resize(vec, vec->capacity * 2);
		
	vec->_data[vec->size++] = element;
}

int vector_remove(vec* vec, void* element)
{
	int index = vector_find(vec, element);
	if (index > -1)
	{
		vector_removeat(vec, index);
		return 1;
	}
	
	return 0;
}

void vector_removeat(vec* vec, unsigned int index)
{
	assert(index < vec->size);

	vec->_data[index] = NULL;

	for (int i = index; i < vec->size - 1; i++)
	{
		vec->_data[i] = vec->_data[i + 1];
	}

	vec->size--;
	if (vec->size > 0 && vec->size == vec->capacity / 4)
	{
		vector_resize(vec, vec->capacity / 2);
	}
}

int vector_find(vec* vec, void* element)
{
	for (int i = 0; i < vec->size; i++)
		if (element == vec->_data[i])
			return i;

	return -1;
}

void vector_free(vec* vec)
{
	free(vec->_data);
	free(vec);
}