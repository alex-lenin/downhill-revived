/* UNIT 0x0022F600 0x0022F870 */
typedef unsigned int u128 __attribute__((mode(TI)));

extern char D_2B10[], D_2C20[], D_1638[], D_17E0[];
extern char D_7F0[], D_948[], D_6F0[], D_280[];
extern char D_78[], D_1430[], D_1050[], D_12F0[];
extern char D_C20[], D_A90[], D_28D0[], D_2738[];

extern int func_0023D868(void *, void *, void *);
extern int func_00242D70(void);

#define U32(offset) (*(unsigned int *)((char *)arg0 + (offset)))
#define U64(offset) (*(long *)((char *)arg0 + (offset)))
#define U128(offset) (*(u128 *)((char *)arg0 + (offset)))

void func_0022F600(void *arg0)
{
    U32(0x120) = ((unsigned int)D_1638 >> 3) | 0x15000000;
    U32(0x124) = ((unsigned int)D_17E0 >> 3) | 0x15000000;
    U32(0x128) = ((unsigned int)D_7F0 >> 3) | 0x15000000;
    U32(0x12C) = ((unsigned int)D_948 >> 3) | 0x15000000;
    U32(0x130) = ((unsigned int)D_6F0 >> 3) | 0x15000000;
    U32(0x134) = ((unsigned int)D_280 >> 3) | 0x15000000;
    U32(0x138) = ((unsigned int)D_78 >> 3) | 0x15000000;
    U32(0x13C) = ((unsigned int)D_1430 >> 3) | 0x15000000;
    U32(0x140) = ((unsigned int)D_1050 >> 3) | 0x15000000;
    U32(0x144) = ((unsigned int)D_12F0 >> 3) | 0x15000000;
    U32(0x148) = ((unsigned int)D_C20 >> 3) | 0x15000000;
    U32(0x14C) = ((unsigned int)D_A90 >> 3) | 0x15000000;
    U32(0x150) = ((unsigned int)D_28D0 >> 3) | 0x15000000;
    U32(0x154) = ((unsigned int)D_2738 >> 3) | 0x15000000;
    U32(0x158) = ((unsigned int)D_2C20 >> 3) | 0x15000000;
    U32(0x15C) = ((unsigned int)D_2B10 >> 3) | 0x15000000;

    U128(0x100) = 0;
    U128(0x190) = 0;
    U128(0x1E0) = 0;
    U128(0x280) = 0;
    U128(0x1A0) = 0;
    U64(0x1C0) = 0;
    U32(0x10C) = 0x6C000096;
    U32(0x19C) = 0x6C030006;
    U32(0x1EC) = 0x6C020004;
    U32(0x28C) = 0x6C010014;
    U32(0x1AC) = 0x50000002;
    U64(0x1C8) = 8;
    U64(0x1B0) = 0x1000000000008001L;
    U64(0x1B8) = 14;
    U32(0x84) = func_0023D868((char *)arg0 + 0x160, (char *)arg0 + 0x164, (char *)arg0 + 0x168);
    U32(0x8C) = func_00242D70();
    U32(0x2C8) = 0x34;
    U32(0x2DC) = 0;
    U32(0x2D8) = 0x34;
    U32(0x2EC) = 0;
    U32(0x2E8) = 0x34;
    U32(0x2FC) = 0;
    U32(0x2F8) = 0x34;
    U32(0x2CC) = 0;
}
