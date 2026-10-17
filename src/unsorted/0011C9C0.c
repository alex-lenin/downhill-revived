/* UNIT 0x0011C9C0 0x0011CA78 */
typedef struct Item {
    char pad[0x270];
} Item;

typedef struct Obj {
    Item items[30];
    void *x4920[30];
    int x4998[30];
    int x4A10;
    int x4A14;
    int x4A18[30];
    int x4A90;
    int x4A94;
    int x4A98;
    int x4A9C[13];
    float x4AD0[13];
} Obj;

extern void func_0011D270(void *);
extern const float D_0029C15C[];

void func_0011C9C0(Obj *obj)
{
    int i;
    int j;
    float f;

    for (i = 0; i < 30; i++) {
        func_0011D270(&obj->items[i]);
        obj->x4920[i] = 0;
        obj->x4998[i] = 0;
        obj->x4A18[i] = 0;
    }

    f = D_0029C15C[0];
    obj->x4A10 = 1;
    obj->x4A14 = 0;
    obj->x4A90 = 0;
    obj->x4A94 = 0;
    obj->x4A98 = 1;

    for (j = 0; j < 13; j++) {
        obj->x4AD0[j] = f;
        obj->x4A9C[j] = -1;
    }
}
