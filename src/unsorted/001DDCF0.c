/* UNIT 0x001DDCF0 0x001DDD30 */
typedef struct Obj {
    char pad0[0x88]; int x88;
    char pad8c[0x17c - 0x8c]; int x17c;
    char pad180[0x218 - 0x180]; int x218;
} Obj;
extern int func_00222B08(int, int);
Obj *func_001DDCF0(Obj *obj) {
    obj->x218 = 0;
    obj->x17c = -1;
    obj->x88 = func_00222B08(0, 10);
    return obj;
}
