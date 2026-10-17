/* UNIT 0x00161418 0x00161450 */
typedef struct Obj {
    char pad0[0x10];
    int x10;
    int x14;
    int x18;
} Obj;

void func_00161418(Obj *obj, unsigned int value) {
    if (value - 0x332C < 100U) {
        obj->x10++;
        obj->x14++;
    } else {
        obj->x18++;
    }
}
