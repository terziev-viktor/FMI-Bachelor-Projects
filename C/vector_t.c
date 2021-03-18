#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    size_t data_size;
    size_t length;
    char data[];
} vector_t;

typedef struct
{
    vector_t v;
    bool (*pushbackorpushfront)(void*, void*, size_t);
} vector_pusher_t;

vector_t * VECTOR_CAST( void * self ) 
{
    return (vector_t*)(((char*)(self)) - offsetof(vector_t, data));
}

void * vector_create(size_t element_size)
{
    vector_t * self = malloc(sizeof(vector_t) + 500);
    self->data_size = 500;
    self->length = 0;
    return self->data;
}

void vector_destroy(void * * self_ref)
{
    if(*self_ref != NULL)
    {
        vector_t * self = VECTOR_CAST(*self_ref);
        free(self);
        *self_ref = NULL;
    }
}

size_t vector_length(void * obj)
{
    vector_t * self = VECTOR_CAST(obj);
    return self->length;
}   

size_t vector_capacity(void * obj, size_t sizeof_e)
{
    vector_t * self = VECTOR_CAST(obj);
    return self->data_size / sizeof_e;
}

bool vector_push_back(void * v, void * e, size_t sizeof_e)
{
    vector_t * self = VECTOR_CAST(v);
    if(self->length * sizeof_e < self->data_size)
   { 
        memcpy(self->data + self->length * sizeof_e, e, sizeof_e);
        self->length++;
        return true;
    }
    else
    {
        return false;
    }
}

#define vector_add(v, e) (v)[VECTOR_CAST((v))->length++] = (e)
#define vector_begin(v) (v)
#define vector_end(v) (&(v)[VECTOR_CAST((v))->length])

void * vector_find_if(void * begin, void * end, size_t s, bool (*f)(void *))
{
    for(char * iter = begin; iter != end; iter += s)
    { 
        if(f(iter))
        {
            return iter; 
        }
    }
    return end;
}

void * vector_create_by_filter(void * begin, void * end, size_t s, bool (*f)(void *))
{
    void * filtered = vector_create(s);
    while(begin != end)
    {
        begin = vector_find_if(begin, end, s, f);
        if(begin != end)
        {
            vector_push_back(filtered, begin, s);
        }
        begin += s;
    } 
    return filtered;
}

typedef struct
{
    int a;
    int b;
} pair_t;

bool is_even_pair(void * pair_ref)
{
    pair_t * pair = (pair_t*) pair_ref;
    return (pair->a % 2 == 0) && (pair->b % 2 == 0);
}

int main()
{
    pair_t * pairs = vector_create(sizeof(pair_t));
    vector_add(pairs, ((pair_t){3,22}));
    vector_add(pairs, ((pair_t){4,6}));
    vector_add(pairs, ((pair_t){1,5}));
    vector_add(pairs, ((pair_t){4,8}));
    vector_add(pairs, ((pair_t){4,8}));
    vector_add(pairs, ((pair_t){4,8}));
    vector_add(pairs, ((pair_t){4,8}));

    {
        pair_t * even_pairs = vector_create_by_filter(vector_begin(pairs), vector_end(pairs), sizeof(pair_t), is_even_pair);

        printf("found %d even pairs:\n", vector_length(even_pairs));

        for(pair_t * i = even_pairs; i != vector_end(even_pairs); ++i)
        {
            printf("{%d, %d}\n", i->a, i->b);
        }

        vector_destroy((void**) &even_pairs);
    }
    vector_destroy((void**)&pairs);
}
