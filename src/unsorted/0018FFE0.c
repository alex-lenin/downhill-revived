/* UNIT 0x0018FFE0 0x001900C8 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef union Vec {
    u128 bits;
    float f[4];
} Vec;

typedef struct Mtx {
    u128 row[4];
} Mtx;

typedef struct Item {
    char pad0[4];
    char x4;
    char pad5;
    char x6;
    char pad7[9];
    Vec x10;
    char pad20[0x20];
    Mtx x40;
} Item;

typedef struct State {
    char pad0[0x20];
    int x20;
} State;

extern Mtx *func_0023CF58(int id);
extern Vec *func_0023CF78(int id);

void func_0018FFE0(State *state, Item *item, Vec *offset)
{
    Vec v;
    Mtx *m;
    int flag;

    if (item == 0) {
        return;
    }
    m = &item->x40;
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
        : "=m"(m->row[0]), "=m"(m->row[1]), "=m"(m->row[2]), "=m"(m->row[3])
        : "r"(func_0023CF58(state->x20))
        : "$8", "$9", "$10", "$11");
    /* vector copy (as sceVu0CopyVector) */
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(item->x10) : "r"(func_0023CF78(state->x20)) : "$2");
    __asm__ volatile (
        "lqc2 $vf11, 0x0(%1)\n\t"
        "lqc2 $vf12, 0x0(%2)\n\t"
        "lqc2 $vf13, 0x10(%2)\n\t"
        "lqc2 $vf14, 0x20(%2)\n\t"
        "vmulax.xyz ACC, $vf12, $vf11x\n\t"
        "vmadday.xyz ACC, $vf13, $vf11y\n\t"
        "vmaddz.xyz $vf11, $vf14, $vf11z\n\t"
        "sqc2 $vf11, 0x0(%0)"
        : : "r"(&v), "r"(offset), "r"(m) : "memory");
    item->x10.f[0] += v.f[0];
    item->x10.f[1] += v.f[1];
    item->x10.f[2] += v.f[2];
    flag = 1;
    item->x6 = flag << state->x20;
    item->x4 = flag;
}
