/* UNIT 0x00198368 0x001983B0 */
typedef struct Obj {
    char pad0[0x460];
    int x460;
    char pad464[0x468 - 0x464];
    int x468;
    char pad46C[0x4A8 - 0x46C];
    int x4A8;
} Obj;

/* 8-byte .sbss object, so its address is formed by the assembler's la macro */
extern long D_0029DE68;
extern void func_0018E6B8(Obj *obj, int n, void *p);

void func_00198368(Obj *obj)
{
    obj->x460 = 0x40;
    obj->x468 = 0xC;
    func_0018E6B8(obj, 0x10, &D_0029DE68);
    obj->x4A8 = 5;
}
