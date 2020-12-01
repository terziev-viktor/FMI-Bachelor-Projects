#include <stdio.h>
#include <stdlib.h>

typedef struct virtual_t_decl virtual_table;

typedef struct shape_t_decl
{
    virtual_table * vtable;
    int x;
    int y;
} shape_t;

typedef struct circle_t_decl
{
    shape_t shape;
    size_t radius;
} circle_t;

void shape_print(shape_t * this)
{
    printf("[%d, %d]", this->x, this->y);
}

void circle_print(shape_t * this)
{
    shape_print(this);
    circle_t * this_circle = (circle_t *) this;

    printf(" radius = %d", this_circle->radius);
}

struct virtual_t_decl
{
    void (*print)(shape_t *);
} shape_virtual_table, circle_virtual_table;

void init_virtual_tables()
{
    shape_virtual_table.print = shape_print;
    circle_virtual_table.print = circle_print;
}

int main()
{
    init_virtual_tables();

    circle_t c;
    c.shape.x = 20;
    c.shape.y = 30;
    c.radius = 420;
    c.shape.vtable = &circle_virtual_table;

    shape_t * ashape = (shape_t *) &c;
    ashape->vtable->print(ashape);
}
