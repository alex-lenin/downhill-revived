/* UNIT 0x00190DB0 0x00190E48 */
struct Obj {
    char pad0[0x13C];
    int *x13C;
    int *x140;
    int *x144;
};
extern int func_001FD238(float);

void func_00190DB0(struct Obj *obj, int value) {
    float scale;
    float level;
    int *a;
    int *b;

    scale = (float)value * 0.0125f;
    level = scale * 127.0f;
    a = obj->x13C;
    *a = func_001FD238(level);
    *obj->x140 = func_001FD238(level);
    b = obj->x144;
    *b = func_001FD238(scale * 40.0f);
}
