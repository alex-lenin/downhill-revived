/* UNIT 0x00172758 0x00172820 */
typedef struct Item {
    char pad0[0x5];
    unsigned char x5;
} Item;

typedef struct Elem {
    char pad0[0x130];
} Elem;

typedef struct Obj {
    Elem x0[1];
    char pad130[0x850 - 0x130];
    Item *x850[(0x888 - 0x850) / 4];
    int x888;
    int x88C;
} Obj;

typedef struct Global {
    char pad0[0x160];
    int x160;
} Global;

extern Global D_004B2914;
extern void func_00172580(Obj *);
extern void func_00172BD0(Elem *);

void func_00172758(Obj *obj)
{
    int active = 0;
    int i;

    if (obj->x88C) {
        func_00172580(obj);
        obj->x88C = 0;
    } else {
        for (i = 0; i < obj->x888; i++) {
            func_00172BD0(&obj->x0[i]);
            if (obj->x850[i] && obj->x850[i]->x5 == 1) {
                active = 1;
            }
        }
    }
    D_004B2914.x160 = active;
}
