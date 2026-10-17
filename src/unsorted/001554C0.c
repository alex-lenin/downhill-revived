/* UNIT 0x001554C0 0x001556D8 */
typedef struct Obj {
    char *x0;
    char p4[0x5C];
    float x60, x64, x68;
    char p6C[0x34];
    float xA0, xA4, xA8;
    char pAC[0x18];
    int xC4;
    unsigned long xC8;
} Obj;

extern void func_00221888(void *, void *, void *);
extern void func_001556D8(Obj *, void *);
extern void func_00155A20(Obj *, void *, int);
extern void func_00155BA8(Obj *, void *);
extern void func_001E67A8(void *);
extern void func_001E6008(void *);
extern int func_001E64F0(void *);
extern unsigned long *func_00138558(void *);

static inline int MIN(int a, int b) { return a < b ? a : b; }
static inline int MAX(int a, int b) { return a < b ? b : a; }
static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}
static inline int clamp3(int a, int b, int c) {
    a = MIN(97, a);
    b = MIN(98, b);
    c = MIN(99, c);
    return MAX(c, MAX(b, a));
}

void func_001554C0(Obj *o, int arg) {
    float sum, scale, v[4];
    int c, b, a;

    if ((o->xC8 & 5) == 1) {
        func_00221888(*(char **)(o->x0 + 0x792C) + 0x40, (void *)arg, v);
        scale = o->xA0 * (*(float *)(o->x0 + 0x7924) * 0.0001f);
        __asm__ volatile (
            "lqc2 $vf11,0(%1)\n\t"
            "mfc1 $2,%2\n\t"
            "qmtc2.ni $2,$vf12\n\t"
            "vmulx.xyz $vf11,$vf11,$vf12x\n\t"
            "sqc2 $vf11,%0"
            : "=m"(v) : "r"(v), "f"(scale) : "$2");
        func_001556D8(o, v);
        if (o->xC8 & 4) goto done;
        if (*(unsigned char *)(o->x0 + 0x7B9C) ||
            func_001E64F0(o->x0 + 0x6610))
            func_00155A20(o, v, arg);
        if (o->xC8 & 4) goto done;
        func_00155BA8(o, v);
        if (o->xC8 & 4) goto done;
        if (*(unsigned char *)(o->x0 + 0x7B9D) && o->xC4 == 0) {
            sum = Abs(o->x60) + Abs(o->x64) + Abs(o->x68);
            if (sum > 50.0f) {
                a = (int)(sum - 50.0f);
                b = (int)Abs(o->xA8);
                c = (int)o->xA4;
                o->xC4 = clamp3(a, b, c);
            }
        }
        if (o->xC8 & 4) goto done;
        if ((*func_00138558(o->x0 + 0x40C0) &
             ((unsigned long)0xF800 << 37)) == ((unsigned long)0xF000 << 37))
            func_001E67A8(o->x0 + 0x6610);
        else
            func_001E6008(o->x0 + 0x6610);
    }
done:
    *(unsigned char *)(o->x0 + 0x7B9C) = 0;
    *(unsigned char *)(o->x0 + 0x7B9D) = 0;
}
