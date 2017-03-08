#include "vector.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void index_guard(uint64_t size, uint8_t i) {
   if(i >= size) {
       fprintf(stderr, "Array out of bounds with index: %u and size %lu", i, size);
       exit(-1);
   }
}

vec_t* vec_init() {
    vec_t* vector = malloc(sizeof(vec_t));
    vector->size = 0;
    vector->alloc_size = 1;
    vector->data = calloc(vector->alloc_size, sizeof(uint8_t));

    return vector;
}
void vec_push_back(vec_t* vec, uint8_t num) {
    // Expand first only if neccessary to size alloc_size*2
    if(vec->size >= vec->alloc_size) {
        vec->alloc_size *= 2;
        vec->data = realloc(vec->data, sizeof(uint8_t)*vec->alloc_size);
    }

    // Increment size and store
    vec->data[vec->size++] = num;
}

uint8_t vec_get_val(vec_t* vec, uint64_t i) {
    index_guard(i, vec->size);

   return vec->data[i];
}


void vec_set_val(vec_t* vec, uint64_t i, uint8_t val) {
    index_guard(i, vec->size);
    
    vec->data[i] = val;
}


uint8_t* vec_get(vec_t* vec) {
    return vec->data;
}


void vec_destoy(vec_t* vec) {
    free(vec->data);
    free(vec);
}
