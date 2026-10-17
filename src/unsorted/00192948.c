/* UNIT 0x00192948 0x00192990 */
typedef struct Entry { int x0; int x4; int x8; int xC; } Entry;
typedef struct Obj { int x0; int x4; int x8; int xC; Entry entries[34]; int x230; char pad234[0x14]; int x248; unsigned char x24C; } Obj;
void func_00192948(Obj *obj) {
    int *entry = &obj->entries[0].x4;
    int count = 33;
    obj->x4 = 0;
    obj->x8 = 0;
    obj->xC = 0;
    do { entry[-1] = 0; --count; entry[0] = 0; entry += 4; } while (count >= 0);
    obj->x248 = 0;
    obj->x24C = 1;
    obj->x230 = 0;
}
