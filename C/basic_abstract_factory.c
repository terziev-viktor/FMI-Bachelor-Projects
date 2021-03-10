#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    void (*Execute)();
} ICommand_VTable;

typedef struct
{
    ICommand_VTable vtable;
} ICommand;

/*virtual*/ void Execute(ICommand * this)
{
    if(this != NULL)
    this->vtable.Execute();
}

typedef struct
{
    ICommand * (*Create)();
} ICommand_Creator_VTable;

typedef struct
{
    ICommand_Creator_VTable vtable;
    const char * name;
} ICommand_Creator;

/*virtual*/ ICommand * Create(const ICommand_Creator * this)
{
    return this->vtable.Create();
}

void ICommand_Creator_Init(ICommand_Creator * self);

//===========================================================================

typedef struct
{
    ICommand super;
} PrintHello;

/*override*/ void PrintHello_Execute()
{
    printf("Hello, World!\n");
}

static const ICommand_VTable PrintHello_VTable = { .Execute = PrintHello_Execute };

typedef struct
{
    ICommand_Creator super;
} PrintHello_Creator;

/*override*/ ICommand * PrintHello_Create(ICommand_Creator * self)
{
    ICommand * p = malloc(sizeof(PrintHello));
    p->vtable = PrintHello_VTable;
    return p;
}

static ICommand_Creator_VTable PrintHello_Creator_vtable = { .Create = PrintHello_Create };

typedef struct
{
    size_t creators_count;
    ICommand_Creator * creators[2];
} ICommand_Factory;

ICommand_Factory * new_ICommand_Factory()
{
    ICommand_Factory * the_factory = malloc(sizeof(ICommand_Factory));
    the_factory->creators_count = 0;
    return the_factory;
}

static ICommand_Factory g_Factory;
bool ICommand_Factory_Register_Creator(ICommand_Creator * the_creator)
{
    if(g_Factory.creators_count < 2)
    {
        g_Factory.creators[g_Factory.creators_count++] = the_creator;
        return true;
    }
    else return false;
}

ICommand * ICommand_Factory_CreateByName(ICommand_Factory * self, const char * name)
{
    for(size_t i = 0; i < self->creators_count; ++i)
    {
        if(strcmp(name, self->creators[i]->name) == 0)
        {
            return Create(self->creators[i]);
        }
    }
    return NULL;
}

void ICommand_Creator_Init(ICommand_Creator * self)
{
    ICommand_Factory_Register_Creator(self);
}

static PrintHello_Creator the_PrintHello_Creator;
void PrintHello_Creator_Init()
{
    static bool is_init = false;
    if(is_init == false)
    {
        the_PrintHello_Creator.super.vtable = PrintHello_Creator_vtable;
        the_PrintHello_Creator.super.name = "printhello";
        ICommand_Creator_Init(&the_PrintHello_Creator.super);

        is_init = true;
    }
}

int main()
{
    PrintHello_Creator_Init();

    ICommand * a_command = ICommand_Factory_CreateByName(&g_Factory, "printhello");

    Execute(a_command);
  
    free(a_command);
}
