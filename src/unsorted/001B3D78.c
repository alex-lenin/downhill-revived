/* UNIT 0x001B3D78 0x001B3E08 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Obj {
    char pad0[0x90];
    u128 x90;
    char padA0[0xC0 - 0xA0];
    void *xC0;
    char padC4[0xE0 - 0xC4];
    u128 xE0;
} Obj;

extern u128 D_00768590[];
extern int func_001B1C08(void *);

/* D_00768590[slot] = mtx * obj->xE0 + *offset; obj->x90 = that */
void func_001B3D78(Obj *obj, u128 *mtx, u128 *offset)
{
    u128 tmp;
    int slot;

    slot = func_001B1C08(obj->xC0);
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tlqc2 $vf13, 0x10(%2)\n\tlqc2 $vf14, 0x20(%2)\n\tvmulax.xyz ACC, $vf12, $vf11x\n\tvmadday.xyz ACC, $vf13, $vf11y\n\tvmaddz.xyz $vf11, $vf14, $vf11z\n\tsqc2 $vf11, 0(%0)"
                      : : "r"(&tmp), "r"(&obj->xE0), "r"(mtx) : "memory");
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%0)"
                      : : "r"(&D_00768590[slot]), "r"(&tmp), "r"(offset) : "memory");
    __asm__ volatile ("lq $2, 0(%1)
	sq $2, %0" : "=m"(obj->x90) : "r"(&D_00768590[slot]) : "$2", "memory");
}
