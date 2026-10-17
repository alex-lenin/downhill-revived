/* UNIT 0x00231058 0x002311A0 */
typedef struct Heap { char pad0[0x10]; int x10; int *x14; } Heap;
typedef struct Semaphore { int x0,x4,x8,xC,x10,x14; } Semaphore;
typedef struct Thread { int x0; void (*x4)(void); void *x8; int xC; void *x10; int x14,x18,x1C,x20; } Thread;
extern Heap D_00663B08;
extern Thread D_00736E00;
extern char D_00736E40[], D_002991D0[];
extern int _gp;
extern int D_0029E33C, D_0029E344, D_0029E348, D_0029E34C, D_0029E354, D_0029E35C, D_0029E368;
extern volatile int D_0029E360; /* volatile keeps arg in a pseudo, which fixes the store order */
extern int D_0029E370, D_0029E374, D_0029E378, D_0029E37C;
extern void func_00231628(void);
extern int func_0025A6E0(Semaphore *);
extern int func_0025A4E0(Thread *);
extern void func_00254D08(char *);
extern void func_0025A500(int,int);
static inline int Allocate(int size,int alignment)
{
    int addr = (D_00663B08.x10 - size) & -alignment;
    D_00663B08.x10 = addr;
    return addr;
}
static inline int StartThread(Thread *t, void (*entry)(void), void *stack, int size, int priority)
{
    t->x4 = entry;
    t->x8 = stack;
    t->xC = size;
    t->x14 = priority;
    t->x10 = &_gp;
    t->x20 = 0;
    return func_0025A4E0(t);
}
/* Pushes a heap mark, allocates the 0x41000-byte double buffer, creates two semaphores and starts the thread. */
void func_00231058(int arg)
{
    Semaphore sema;
    Semaphore *ps;
    int addr;
    int *old;
    /* empty loops are scheduling barriers needed to match */
    do {
    } while (0);
    old = D_00663B08.x14;
    addr = (D_00663B08.x10 - 4) & ~3;
    D_00663B08.x10 = addr;
    D_00663B08.x14 = (int *)addr;
    *(int **)addr = old;
    D_0029E360 = arg;
    addr = Allocate(0x41000,64);
    D_0029E354 = 0;
    D_0029E370 = addr;
    D_0029E374 = addr + 0x20800;
    D_0029E35C = 0;
    D_0029E368 = 0;
    D_0029E378 = 0;
    D_0029E37C = 0;
    ps = &sema;
    ps->x8 = 1;
    ps->x4 = 1;
    do {
    } while (0);
    D_0029E348 = func_0025A6E0(ps);
    D_0029E34C = func_0025A6E0(ps);
    D_0029E344 = 0;
    D_0029E33C = StartThread(&D_00736E00, func_00231628, D_00736E40, 0x2000, 2);
    if (D_0029E33C == -1) func_00254D08(D_002991D0);
    func_0025A500(D_0029E33C,0);
}
