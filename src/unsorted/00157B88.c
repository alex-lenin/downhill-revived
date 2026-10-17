/* UNIT 0x00157B88 0x00157BE8 */
typedef struct { char pad[0x10]; char x10[0x110]; float x120; } Obj;
extern void func_00231C10(void *);
extern void func_00231C20(void *, float *);
void func_00157B88(Obj *obj) {
    float vector[4] __attribute__((aligned(16)));
    void *p = obj->x10;
    func_00231C10(p);
    __asm__ volatile("sqc2 $vf0,0(%0)" : : "r"(vector) : "memory");
    vector[2] = obj->x120 * -96.600006f;
    func_00231C20(p, vector);
}
