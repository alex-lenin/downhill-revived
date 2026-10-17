/* UNIT 0x00165DE8 0x00165E68 */
typedef struct { char pad[0x120]; int x120; char pad124[0x7c]; float x1A0; } Obj;
typedef struct { float x0; char pad[0x10]; float x14; char pad18[0xa8]; } Entry;
extern Entry D_003960A0[];
extern void func_00231C10(Obj *);
extern void func_002318F8(Obj *, float);
extern void func_00231930(Obj *, float);
extern void func_00231C20(Obj *, float *);
void func_00165DE8(Obj *obj) {
    float vec[4] __attribute__((aligned(16)));
    Entry *entry = &D_003960A0[obj->x120];
    func_00231C10(obj);
    func_002318F8(obj, entry->x14);
    func_00231930(obj, obj->x1A0);
    __asm__ volatile ("sqc2 $vf0, 0x0(%0)" : : "r"(vec) : "memory");
    vec[2] = entry->x0 * -80.5f;
    func_00231C20(obj, vec);
}
