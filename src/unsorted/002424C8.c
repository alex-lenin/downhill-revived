/* UNIT 0x002424C8 0x002424E0 */
typedef struct Obj {
    char pad0[0x48];
    int x48;
    char pad4C[0x8];
    int x54;
} Obj;

int func_002424C8(Obj *obj) {
    return !(obj->x54 < obj->x48);
}
