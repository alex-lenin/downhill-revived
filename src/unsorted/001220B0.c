/* UNIT 0x001220B0 0x00122120 */
struct Obj { char pad0[0x154]; float x154; char pad158[8]; char x160[0xb0]; float x210; };
extern float *func_00261028(void *);
extern float *func_00261038(void *);
extern float *func_00261030(void *);
void func_001220B0(struct Obj *obj, float value) {
    float scaled = value * 60.0f;
    void *part = obj->x160;
    obj->x210 = value;
    *func_00261028(part) = scaled;
    *func_00261038(part) = scaled;
    *func_00261030(part) = scaled;
    obj->x154 = scaled;
}
