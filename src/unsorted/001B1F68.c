/* UNIT 0x001B1F68 0x001B1F98 */
typedef struct Obj { char bytes[0x110]; } Obj;
int func_001B1F68(Obj *obj, Obj *target) {
    int index;
    for (index = 0; index < 4; index++, obj++) {
        if (target == obj) return index;
    }
    return -1;
}
