/* UNIT 0x00181690 0x001816B0 */
struct Obj {
    char pad0[0x8];
    void *x8;
};

extern void func_002093C8(void *p, float *pos, float *out);

void func_00181690(struct Obj *obj, float *pos, float *out)
{
    func_002093C8(obj->x8, pos, out);
}
