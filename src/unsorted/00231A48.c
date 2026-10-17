/* UNIT 0x00231A48 0x00231A78 */
typedef struct { char pad[0x34]; void *x34; char pad38[0x18]; float x50[4]; } Obj;
void func_00231A48(Obj *obj, void *offset) {
    void *p = obj->x34;
    void *q;
    __asm__ volatile("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0(%0)" : : "r"(p), "r"(offset) : "memory");
    q = obj->x50;
    __asm__ volatile("lqc2 $vf11, 0(%0)\n\tlqc2 $vf12, 0(%1)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0x50(%2)" : : "r"(q), "r"(offset), "r"(obj) : "memory");
}
