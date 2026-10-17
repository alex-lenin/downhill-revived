/* UNIT 0x00175658 0x00175708 */
typedef struct { int x0; char x4; } Item;
typedef struct {
    char pad0[0xF98]; void *xF98;
    char padF9C[0x1E8]; Item *x1184;
    char pad1188[0x98]; int x1220; float x1224, x1228;
    char pad122C[0x98]; int x12C4; int x12C8; int x12CC;
    char pad12D0[0x94]; int x1364;
} Obj;
extern int D_004B26AC[];
extern Item *func_00209108(void);
extern void func_00218278(Item *, void *);
void func_00175658(Obj *obj, int value) {
    int duration;
    obj->x1364 = value;
    if (D_004B26AC[0] != 18 && D_004B26AC[0] != 13 && D_004B26AC[0] != 5) {
        Item *item = func_00209108();
        obj->x1184 = item;
        if (item && obj->xF98) {
            func_00218278(item, obj->xF98);
            obj->x1184->x4 = 1;
        }
        obj->x1224 = 30.0f;
        obj->x1228 = 10.0f;
        obj->x1220 = 0;
    }
    duration = obj->x12CC ? 120 : 90;
    obj->x12C4 = duration - (duration >> 2);
}
