/* UNIT 0x00119120 0x00119150 */
typedef struct Obj {
    char pad0[0xE];
    short xE;
} Obj;
extern float D_0039E818[];
int func_00119120(void *unused, Obj *obj, int scale) {
    int value = obj->xE;
    if (scale) value = (int)(value * D_0039E818[0]);
    return value;
}
