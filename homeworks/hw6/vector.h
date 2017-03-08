#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdint.h>

typedef struct {
    uint64_t size;
    uint64_t alloc_size;
    uint8_t* data;
} vec_t;

vec_t* vec_init();
void vec_push_back(vec_t*, uint8_t);
uint8_t vec_get_val(vec_t*, uint64_t);
void vec_set_val(vec_t*, uint64_t, uint8_t);
uint8_t* vec_get(vec_t*);
void vec_destoy(vec_t*);

#endif
