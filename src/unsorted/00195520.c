/* UNIT 0x00195520 0x00195568 */
typedef struct Item {
    int x0[6];
} Item;

typedef struct Obj {
    char pad0[0x5C];
    Item items[1];
} Obj;

void func_00195520(Obj *obj, int index, Item *item)
{
    obj->items[index] = *item;
}
