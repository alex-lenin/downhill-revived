/* UNIT 0x00159418 0x00159468 */
typedef struct Item {
    char pad0[0x200];
} Item;

typedef struct Obj {
    char pad0[0x10];
    Item items[20];
    char pad2810[0x110];
    int count; /* 0x2920 */
} Obj;

extern void func_001598F0(Item *);

void func_00159418(Obj *obj)
{
    int i;

    if (obj->count > 0) {
        for (i = 0; i < 20; i++) {
            func_001598F0(&obj->items[i]);
        }
    }
}
