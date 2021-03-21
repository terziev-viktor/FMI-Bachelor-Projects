#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

typedef struct
{
    size_t data_size;
    size_t length;
    char data[];
} vector_t;

vector_t * VECTOR_CAST( void * self ) 
{
    return (vector_t*)(((char*)(self)) - offsetof(vector_t, data));
}

void * vector_create_with_capacity(size_t elements_count, size_t of_size)
{
    size_t data_size = elements_count * of_size;
    vector_t * self = malloc(sizeof(vector_t) + data_size);
    if(self == NULL)
    {
        return NULL;
    }

    self->data_size = data_size;
    self->length = 0;
    return self->data;
}

void * vector_create()
{
    return vector_create_with_capacity(0, 0);
}

void * vector_create_with_length(size_t length, size_t element_size)
{
    void * self = vector_create(length, element_size);
    VECTOR_CAST(self)->length = length;
    return self;
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

bool vector_reserve(void * * s, size_t new_length, size_t sizeof_e)
{
    vector_t * self = VECTOR_CAST(*s);

    if(new_length <= self->length)
    {
        return true;
    }

    size_t old_length = self->length;
    size_t new_data_size = new_length * sizeof_e;

    vector_t * self_tmp = NULL;
    self_tmp = realloc(self, sizeof(vector_t) + new_data_size);
    if(self_tmp == NULL)
    {
        return false;
    }
    else self = self_tmp;

    self->data_size = new_data_size;
    *s = self->data;

    return true;
}

bool vector_resize(void * * s, size_t new_length, size_t sizeof_e)
{
    bool result = vector_reserve(s, new_length, sizeof_e);
    vector_t * self = VECTOR_CAST(*s);
    self->length = new_length;
    return true;
}

bool vector_push_back(void * * ref_self_data, void * e, size_t sizeof_e)
{
    vector_t * self = VECTOR_CAST(*ref_self_data);

    if(self->length + 1 > self->data_size / sizeof_e)
    {
        size_t new_length = ((double)self->length * 1.61803398875) + 1;

        vector_reserve(ref_self_data, new_length, sizeof_e);

        self = VECTOR_CAST(*ref_self_data);
    }
    
    memcpy(self->data + self->length * sizeof_e, e, sizeof_e);
    self->length++;
    return true;
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

#define vector_add(v, e) (v)[VECTOR_CAST((v))->length++] = (e)
#define vector_begin(v) (v)
#define vector_end(v) (&(v)[VECTOR_CAST((v))->length])

void * vector_find_if(void * begin, void * end, size_t sizeof_e, bool (*f)(void *))
{
    for(void * iter = begin; iter != end; iter += sizeof_e)
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
    void * filtered = vector_create(1, s);
    while(begin != end)
    { 
        void * iter = vector_find_if(begin, end, s, f);
        if(iter == end)
        {
            return filtered;
        }
        vector_push_back((void**) &filtered, iter, s);
        begin = iter + s;
    }
    return filtered;
}

void vector_transform(void * begin, void * end, void * out_begin, size_t sizeof_e,  void (*f)(void *, void *) )
{
    for(void * iter = begin; iter != end; iter += sizeof_e, out_begin += sizeof_e)
    {
        f(iter, out_begin);
    }
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

void pair_times_rand(void * in_pair_ref, void * out_pair_ref)
{
    pair_t * in_pair = (pair_t *) in_pair_ref;
    pair_t * out_pair = (pair_t *) out_pair_ref;

    out_pair->a = in_pair->a * (rand() % 20 + 1);
    out_pair->b = in_pair->b * (rand() % 20 + 1);
}

int main()
{
    srand(time(0));

    pair_t * pairs = vector_create_with_capacity(1, sizeof(pair_t));
    vector_add(pairs, ((pair_t){3,3}));
    
    {
        pair_t p = { 1, 2 };
        for(size_t i = 0; i < 20; ++i)
        {
            vector_push_back((void**) &pairs, &p, sizeof(pair_t));
        }
    }
    
    vector_transform(pairs, vector_end(pairs), pairs, sizeof(pair_t), pair_times_rand);

    {
        pair_t * even_pairs = vector_create_by_filter(vector_begin(pairs), vector_end(pairs), sizeof(pair_t), is_even_pair);

        for(pair_t * p = vector_begin(even_pairs); p != vector_end(even_pairs); ++p)
        {
            printf("{%d, %d}\n", p->a, p->b);
        }

        vector_destroy((void**) &even_pairs);
    }
    vector_destroy((void**) &pairs);
}
