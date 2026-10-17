/* UNIT 0x0023CFA0 0x0023CFE0 */
typedef struct Obj {
    char pad0[0x40];
    float x40[4][4];
} Obj;

typedef struct Entry {
    Obj *obj;
    char pad4[0x10];
} Entry;

extern Entry D_0075A9D0[];

void func_0023CFA0(float (*m)[4], int i)
{
    Obj *obj = D_0075A9D0[i].obj;

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
        : "=m"(obj->x40[0]), "=m"(obj->x40[1]), "=m"(obj->x40[2]), "=m"(obj->x40[3])
        : "r"(m)
        : "$8", "$9", "$10", "$11");
}
