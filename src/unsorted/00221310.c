/* UNIT 0x00221310 0x00221438 .rodata=0x298E20:0x298E48 -O0 -fno-opt-stack */
typedef struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
} SemaParam;

typedef struct ThreadParam {
    int status;
    void (*entry)(void *);
    void *stack;
    int stackSize;
    void *gpReg;
    int initPriority;
    int currentPriority;
    unsigned int attr;
    unsigned int option;
} ThreadParam;

extern ThreadParam D_006610C8;
extern int D_006610F8;
extern char D_00661100[];
extern char _gp[];
extern int D_00663100, D_00663104, D_00663108, D_0066310C, D_00663110;
extern int D_00279A60;

extern void func_00221608(void *);
extern int func_0025A6E0(SemaParam *param);
extern int func_0025A4E0(ThreadParam *param);
extern int func_0025A500(int thread, int arg);
extern void func_00254D08(const char *fmt, ...);

/* Creates the load bar semaphore and starts the load bar thread. */
void func_00221310(int arg0, int arg1)
{
    SemaParam sema;

    sema.initCount = 1;
    sema.maxCount = 1;
    D_00663100 = func_0025A6E0(&sema);
    D_00279A60 = 1;
    D_00663104 = arg0;
    D_00663108 = arg1;
    D_0066310C = 0;
    D_00663110 = 0;
    D_006610C8.entry = func_00221608;
    D_006610C8.stack = D_00661100;
    D_006610C8.stackSize = 0x2000;
    D_006610C8.initPriority = 0;
    D_006610C8.gpReg = _gp;
    D_006610C8.option = 0;
    D_006610F8 = func_0025A4E0(&D_006610C8);
    if (D_006610F8 == -1) {
        func_00254D08("couldn't create load bar thread \n");
    }
    func_0025A500(D_006610F8, 0);
}
