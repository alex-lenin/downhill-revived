/* UNIT 0x00172D90 0x00172E68 */
typedef struct Board {
    char pad0[0x18];
    float x18;
} Board;

typedef struct Rider {
    char pad0[0x792C];
    Board *x792C;
    char pad7930[0x7A5C - 0x7930];
    int x7A5C;
    char pad7A60[0x7BD0 - 0x7A60];
} Rider;

typedef struct Global {
    Rider riders[10];
    char pad4D620[0x501A8 - 0x4D620];
    int x501A8;
} Global;

typedef struct Obj {
    char pad0[0x8];
    int x8;
    char padC[0x44 - 0xC];
    int x44;
    char pad48[4];
    int x4C;
} Obj;

extern Global D_002AF2C0;
extern float D_0029C8E4[];
extern char D_003B01E0[];
extern float func_00222B68(float, float);
extern int func_00174590(void *, int, float);

void func_00172D90(Obj *obj) {
    float max = D_0029C8E4[0];
    int i;
    int result;

    for (i = 0; i < D_002AF2C0.x501A8; i++) {
        Rider *rider = &D_002AF2C0.riders[i];
        if (rider->x7A5C == 1 && max < rider->x792C->x18) {
            max = rider->x792C->x18;
        }
    }
    max += 600.0f;
    max += func_00222B68(0.0f, 300.0f);
    if (obj->x44) {
        obj->x4C = result = func_00174590(D_003B01E0, obj->x44, max);
        if (result == obj->x44) {
            obj->x8 = 1;
        } else {
            obj->x8 = 3;
        }
    }
}
