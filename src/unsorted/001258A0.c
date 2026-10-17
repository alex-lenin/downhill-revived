/* UNIT 0x001258A0 0x00125900 */
struct Obj { char pad0[0x1C4]; float x1C4; char pad1C8[0x18]; char x1E0[1]; };
extern float *func_00261028(void *);
extern float *func_00261038(void *);
extern float *func_00261030(void *);
void func_001258A0(struct Obj *obj, float value) {
    void *sub = obj->x1E0;
    *func_00261028(sub) = value;
    *func_00261038(sub) = value;
    *func_00261030(sub) = value;
    obj->x1C4 = value;
}
