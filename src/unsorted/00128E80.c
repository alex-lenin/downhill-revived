/* UNIT 0x00128E80 0x00128F50 */
typedef struct Slot {
    char pad0[0x1A0];
} Slot;

typedef struct Obj {
    Slot slots[30];
    int x30C0[30];
    int x3138[30];
    int x31B0;
    float x31B4;
    float x31B8;
    float x31BC;
    float x31C0;
    int x31C4;
    int x31C8;
    int x31CC[30];
    int x3244;
    int x3248;
    int x324C;
    int x3250;
    int x3254;
    int x3258;
} Obj;

extern void func_001293D0(Slot *slot);

void func_00128E80(Obj *obj) {
    int i;

    for (i = 0; i < 30; i++) {
        func_001293D0(&obj->slots[i]);
        obj->x30C0[i] = 0;
        obj->x3138[i] = 0;
        obj->x31CC[i] = 0;
    }
    obj->x31B4 = 0.65999997f;
    obj->x31B8 = 0.90444446f;
    obj->x31BC = 1.2f;
    obj->x31C0 = 1.4f;
    obj->x31B0 = 0;
    obj->x31C4 = 0;
    obj->x31C8 = 0;
    obj->x3250 = 0;
    obj->x3254 = 0;
    obj->x324C = 0;
    obj->x3244 = 0;
    obj->x3248 = 0;
    obj->x3258 = 1;
}
