/* UNIT 0x0011AD18 0x0011AD78 */
typedef struct Item { char pad[0x7BD0]; } Item;
typedef struct ObjB { char pad[0x450]; } ObjB;
typedef struct ObjC { char pad[0x110]; } ObjC;

typedef struct World {
    Item items[10];
    ObjB b[9];
    ObjC c[4];
    Item *cOwner[4];
    Item *bOwner[9];
} World;

extern void func_0012C130(Item *, int, ObjC *);

void func_0011AD18(World *w, int idx, int item)
{
    Item *it = &w->items[item];

    if (it != 0) {
        w->cOwner[idx] = it;
        func_0012C130(it, idx, &w->c[idx]);
    }
}
