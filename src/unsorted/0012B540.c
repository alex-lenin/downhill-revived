/* UNIT 0x0012B540 0x0012B5B0 */
struct Obj { char pad0[0xc4]; float xc4; char padc8[8]; char xd0[0x7c]; float x14c; };
extern float *func_00261028(void *);
extern float *func_00261038(void *);
extern float *func_00261030(void *);
void func_0012B540(struct Obj *obj, float value) {
    float scaled = value * 60.0f;
    void *part = obj->xd0;
    obj->x14c = value;
    *func_00261028(part) = scaled;
    *func_00261038(part) = scaled;
    *func_00261030(part) = scaled;
    obj->xc4 = scaled;
}
