/* UNIT 0x001BE3D8 0x001BE468 */
typedef struct Slot {
    char pad0[0xC];
} Slot;

typedef struct Obj {
    int x0;
    int x4;
    int x8[30];
    int x80;
    char pad84[0x8];
    Slot x8C[30];
    int x1F4;
    int x1F8;
    int x1FC;
} Obj;

extern void func_001BE628(Slot *slot, int mode);

void func_001BE3D8(Obj *obj, int arg)
{
    int i;

    obj->x4 = arg;
    obj->x80 = 0;
    for (i = 0; i < 30; i++) {
        obj->x8[i] = 0x7FFF;
        func_001BE628(&obj->x8C[i], 3);
    }
    obj->x1F8 = 7;
    obj->x1FC = 0;
    obj->x1F4 = -1;
}
