/* UNIT 0x0011AD80 0x0011ADE0 */
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

extern void func_0012C398(Item *, int, ObjB *);

void func_0011AD80(World *w, int idx, int item)
{
    Item *it = &w->items[item];

    if (it != 0) {
        w->bOwner[idx] = it;
        func_0012C398(it, idx, &w->b[idx]);
    }
}
