/* UNIT 0x00196038 0x001960F0 */
typedef struct Entry {
    char pad0[0x14];
} Entry;

typedef struct Obj {
    int x0[6];
    Entry x18[150];
    int xBD0[6];
    int xBE8;
    int xBEC;
    int xBF0;
    int xBF4;
    int xBF8;
    int xBFC;
    int xC00;
    int xC04;
    float xC08;
} Obj;

extern void func_00196840(Entry *e);

void func_00196038(Obj *obj)
{
    int i;

    for (i = 0; i < 6; i++) {
        obj->x0[i] = 0;
        obj->xBD0[i] = 0;
    }
    for (i = 0; i < 150; i++) {
        func_00196840(&obj->x18[i]);
    }
    obj->xC00 = 0;
    obj->xBE8 = 60;
    obj->xBEC = 60;
    obj->xBF0 = 60;
    obj->xBF4 = 1800;
    obj->xBF8 = 60;
    obj->xBFC = 60;
    obj->xC04 = 25;
    obj->xC08 = 20.0f;
}
