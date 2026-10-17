/* UNIT 0x001179D0 0x00117A10 */
struct Obj { int x0, x4, x8; char padc[8]; short entries[1024]; };
void func_001179D0(struct Obj *obj) {
    int i;
    short *entry;
    int value = -1;
    obj->x0 = 0;
    obj->x4 = 0;
    obj->x8 = 0;
    entry = &obj->entries[1023];
    i = 1023;
    do {
        *entry = value;
        i--;
        entry--;
    } while (i >= 0);
}
