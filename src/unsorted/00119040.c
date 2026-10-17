/* UNIT 0x00119040 0x001190B0 */
typedef struct Obj { char pad0[0x1004C]; int x1004C; } Obj;
typedef struct Item { char pad0[0xE]; short xE; } Item;
extern Item *func_001177F8(Obj *,int);
int func_00119040(Obj *obj) {
    int index = obj->x1004C - 2;
    while (index > 0) {
        int value = func_001177F8(obj,index)->xE;
        if (value >= 2) return value;
        index--;
    }
    return 0;
}
