/* UNIT 0x0019B0B8 0x0019B100 */
typedef struct Obj {
    char pad0[0x500];
    int x500;
    int x504;
    int x508;
    signed char rows[1][13];
} Obj;

int func_0019B0B8(Obj *obj, int value) {
    int found = 0;
    int i;
    for (i = 0; i < 12; i++) {
        if (obj->rows[obj->x500][i] == value) {
            found = 1;
        }
    }
    return found;
}
