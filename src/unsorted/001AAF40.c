/* UNIT 0x001AAF40 0x001AAFC0 */
typedef struct Model {
    float x0;
    char pad4[0xC];
    float x10;
    float x14;
    char pad18[0x28];
    char x40[0x100]; /* matrix work area */
} Model;

typedef struct Obj {
    char pad0[0x20];
    Model *x20;
} Obj;

extern void func_00231C10(void *m);
extern void func_002318F8(void *m, float angle);
extern void func_00231930(void *m, float angle);
extern void func_00231C20(void *m, float *offset);

void func_001AAF40(Obj *obj)
{
    float vector[4] __attribute__((aligned(16)));
    void *m = obj->x20->x40;

    func_00231C10(m);
    func_002318F8(m, obj->x20->x14);
    func_00231930(m, obj->x20->x10);
    __asm__ volatile("sqc2 $vf0,0(%0)" : : "r"(vector) : "memory");
    vector[2] = obj->x20->x0 * -80.5f;
    func_00231C20(m, vector);
}
