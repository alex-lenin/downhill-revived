/* UNIT 0x0023B248 0x0023B2C0 */
typedef struct SemaParam { int currentCount, maxCount, initCount, numWaitThreads; unsigned int attr, option; } SemaParam;
extern "C" int func_0025A6E0(SemaParam *);

struct Obj {
    int x0, x4, x8;
    char padC[0xC];
    int x18;
    char pad1C[0x24];
    int x40;
    int x44;
    long x48;
    int x50, x54;
    int func_0023B248(int a, int b, int c, int d, int e);
    int func_0023B2C0();
};

int Obj::func_0023B248(int a, int b, int c, int d, int e) {
    SemaParam param;
    x0 = a;
    x50 = d;
    x54 = e;
    x4 = (b & 0x0FFFFFFF) | 0x20000000;
    x8 = c;
    x18 = c << 11;
    param.initCount = 1;
    param.maxCount = 1;
    x40 = func_0025A6E0(&param);
    this->func_0023B2C0();
    x48 = 0;
    return 1;
}
