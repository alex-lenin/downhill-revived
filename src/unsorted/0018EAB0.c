/* UNIT 0x0018EAB0 0x0018EAC8 */
typedef struct Item {
    char pad0[0x4B9];
    unsigned char x4B9;
} Item;

typedef struct Obj {
    char pad0[0x500];
    Item *x500[1];
} Obj;

void func_0018EAB0(Obj *obj, int index, Item *item)
{
    obj->x500[index] = item;
    item->x4B9 = 0;
}
