#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint16_t Signal_t;
enum ReservedSignals
{
    INIT_SIGNAL,
    USER_SIGNAL
};

typedef struct
{
    Signal_t signal;
} Event_t;

typedef struct ActiveObject_t ActiveObject_t;

typedef void (*DispatchHandler)(ActiveObject_t * this, const Event_t * const e);

struct ActiveObject_t 
{
    void * thread;
    void * thread_cb;
    void * queue;
    /*virtual*/ DispatchHandler dispatch;
};

void ActiveObject_init(ActiveObject_t * const this, DispatchHandler dispatch)
{
    this->thread = NULL;
    this->thread_cb = NULL;
    this->queue = NULL;
    this->dispatch = dispatch;
}

void ActiveObject_post(ActiveObject_t * const this, const Event_t * const e)
{
    switch(e->signal)
    {
        case INIT_SIGNAL:
        {
            printf("INIT EVENT\n");
            break;
        }
        case USER_SIGNAL:
        {
            printf("USER EVENT\n");
            break;
        }
        default:
        {
            printf("Unknown Event\n");
            break;
        }

    }
}

void qRecieve(void * q, Event_t * * e)
{
    static Event_t mockEvent = { USER_SIGNAL };
    printf("Receiving a USER EVENT\n");
    *e = &mockEvent;
}

static void ActiveObject_eventLoop(ActiveObject_t * this)
{
    static const Event_t initEvent = { INIT_SIGNAL };
    this->dispatch(this, &initEvent);

    while(true)
    {
        Event_t * e = NULL;
        qRecieve(this->queue, &e);
        this->dispatch(this, e);
    }
}

void ActiveObject_start(ActiveObject_t * const this, uint8_t prio, Event_t ** qSto, size_t qLen, void * stack, size_t stackDepth)
{
    // ...
    ActiveObject_eventLoop(this);
}

void DispatchHandler_Print(ActiveObject_t * this, const Event_t * const e)
{
    printf("Dispatching event: ");
     switch(e->signal)
    {
        case INIT_SIGNAL:
        {
            printf("INIT EVENT\n");
            break;
        }
        case USER_SIGNAL:
        {
            printf("USER EVENT\n");
            break;
        }
        default:
        {
            printf("Unknown Event\n");
            break;
        }
    }
}

int main()
{
    ActiveObject_t obj;
    ActiveObject_init( &obj, DispatchHandler_Print);
    ActiveObject_start( &obj, 9, NULL, 10, NULL, 10 );
}
