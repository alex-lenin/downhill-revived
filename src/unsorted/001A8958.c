/* UNIT 0x001A8958 0x001A8A10 */
typedef struct Item {
    char pad0[0x14];
} Item;

typedef struct Obj {
    char pad0[0x8];
    int x8;
    char padC[0x4];
    char *x10;
    char *x14;
    char pad18[0x10];
    int x28;
    char pad2C[0xC];
    int x38;
    int x3C;
    int x40;
    unsigned char x44;
    char pad45[2];
    unsigned char x47;
    unsigned char x48;
    unsigned char x49;
    char pad4A[6];
    int x50;
    int x54;
    int x58;
    Item items[120];
    char pad9BC[4];
    int x9C0;
} Obj;

extern char D_004412C0[];
extern int D_0029DE78;

extern void func_001AA0F8(Obj *obj);
extern void func_001A87C0(Item *item);
extern void func_001A9038(Obj *obj);

void func_001A8958(Obj *obj)
{
    int i;

    func_001AA0F8(obj);
    obj->x44 = 0;
    obj->x49 = 0;
    obj->x14 = obj->x10 = D_004412C0;
    obj->x50 = 0;
    obj->x54 = 0;
    obj->x58 = 0;
    obj->x40 = 0;
    obj->x48 = 0;
    obj->x3C = 0;
    obj->x8 = 3;
    obj->x9C0 = -1;
    for (i = 0; i < 120; i++) {
        func_001A87C0(&obj->items[i]);
    }
    func_001A9038(obj);
    obj->x28 = 0;
    obj->x47 = 0;
    obj->x38 = 0;
    D_0029DE78 = 0;
}
