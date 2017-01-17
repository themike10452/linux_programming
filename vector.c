#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vector.h"

vec* vector(unsigned int elemsize, unsigned int capacity)
{
	vec* vec = malloc(sizeof(vec));
	if (!vec) return NULL;

	vec->_data = malloc(elemsize * capacity);
	if (!vec->_data) return NULL;

	vec->_elemsize = elemsize;
	vec->size = 0;
	vec->capacity = capacity;

	return vec;
}

void* vector_get(vec* vec, unsigned int index)
{
	assert(index < vec->size);
	return (void*)*(vec->_data + (index * vec->_elemsize));
}

int vector_getp(vec* vec, void* out, unsigned int index)
{
	if (index >= vec->size) return -1;
	memcpy((int*)out, vec->_data + (index * vec->_elemsize), vec->_elemsize);
	return 0;
}

void vector_push(vec* vec, void* element, unsigned int elemsize)
{
	assert(elemsize <= vec->_elemsize);
	memcpy(vec->_data + (vec->size * vec->_elemsize), element, vec->_elemsize);
	vec->size++;
}

void vector_free(vec* vec)
{
	free(vec->_data);
	free(vec);
}