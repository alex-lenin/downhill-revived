#ifndef DHD_TIMER_H
#define DHD_TIMER_H

typedef unsigned int (*TimerCallback)(void *);
typedef unsigned int (*TimerCallbackNoArg)(void);
typedef struct TimerNode {
    struct TimerNode *prev, *next;
    unsigned int due;
    TimerCallback callback;
    void *context;
} TimerNode;

typedef struct TimerPool {
    TimerNode *prev, *active;
    TimerNode freeHead;
#ifdef __cplusplus
    void func_00236D80(TimerNode *);
#endif
} TimerPool;

typedef char TimerNode_size_check[sizeof(TimerNode) == 0x14 ? 1 : -1];
typedef char TimerPool_size_check[sizeof(TimerPool) == 0x1C ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void func_00236A30(TimerPool *, int);
TimerNode *func_00236B28(TimerPool *, TimerCallback, void *, int);
TimerNode *func_00236B98(TimerPool *, TimerCallbackNoArg, int);
void func_00236C00(TimerPool *);
int func_00236C60(TimerPool *);
TimerNode *func_00236D20(TimerPool *);
void func_00236D80(TimerPool *, TimerNode *);
#ifdef __cplusplus
}
#endif

#endif
