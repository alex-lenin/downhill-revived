/* UNIT 0x0010A528 0x0010A5D0 */
#include "timer.h"
typedef unsigned int u128 __attribute__((mode(TI)));

struct Mtx { u128 row[4]; };
struct Slot { void *x0; int pad4[3]; struct Mtx x10; u128 x50; unsigned char used; };
struct Obj { int x0; float time; };

extern int D_00759780[];
extern struct Slot *func_0010A5D0(void);
extern void func_00260848(void);

void func_0010A528(struct Obj *obj, struct Mtx *mtx, u128 *vec)
{
    struct Slot *s = func_0010A5D0();

    if (s != 0) {
        s->x0 = obj;
        /* 64-byte matrix copy (as sceVu0CopyMatrix) */
        __asm__ volatile (
            "lq $8, 0x0(%4)\n\t"
            "lq $9, 0x10(%4)\n\t"
            "lq $10, 0x20(%4)\n\t"
            "lq $11, 0x30(%4)\n\t"
            "sq $8, %0\n\t"
            "sq $9, %1\n\t"
            "sq $10, %2\n\t"
            "sq $11, %3"
            : "=m"(s->x10.row[0]), "=m"(s->x10.row[1]), "=m"(s->x10.row[2]), "=m"(s->x10.row[3])
            : "r"(mtx)
            : "$8", "$9", "$10", "$11");
        /* vector copy (as sceVu0CopyVector) */
        __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(s->x50) : "r"(vec) : "$2");
        func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00260848), s, (int)(obj->time * 60.0f));
    }
}
