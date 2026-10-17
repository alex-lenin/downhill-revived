/* UNIT 0x0018DB10 0x0018DB20 */
typedef struct Obj {
    char pad0[0x480];
    int x480;
} Obj;

int func_0018DB10(Obj *obj, int value) {
    return obj->x480 < value;
}
