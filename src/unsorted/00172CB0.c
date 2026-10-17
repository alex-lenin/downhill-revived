/* UNIT 0x00172CB0 0x00172D90 */
typedef struct Body {
    char pad0[0x18];
    float height;
} Body;

typedef struct Racer {
    char pad0[0x792C];
    Body *body;
    char pad7930[0x7A5C - 0x7930];
    int active;
    char pad7A60[0x7BD0 - 0x7A60];
} Racer;

typedef struct Global {
    Racer racers[10];
    char pad4D620[0x501A8 - 0x4D620];
    int racerCount;
} Global;

typedef struct Elem {
    char pad0[0x8];
    int state;
    char padC[0x44 - 0xC];
    int x44;
    char pad48[0x4C - 0x48];
    int x4C;
} Elem;

extern Global D_002AF2C0;
extern float D_0029C8E0[];
extern char D_003B01E0[];
extern float func_00222B68(float min, float max);
extern int func_00174478(void *table, int id, float value);

void func_00172CB0(Elem *elem)
{
    Racer *racer;
    float lowest;
    int i;
    int result;

    lowest = D_0029C8E0[0];
    for (i = 0; i < D_002AF2C0.racerCount; i++) {
        racer = &D_002AF2C0.racers[i];
        if (racer->active == 1 && racer->body->height < lowest) {
            lowest = racer->body->height;
        }
    }
    lowest -= 1100.0f;
    lowest -= func_00222B68(0.0f, 300.0f);
    if (elem->x44 != 0) {
        result = func_00174478(D_003B01E0, elem->x44, lowest);
        elem->x4C = result;
        if (result == elem->x44) {
            elem->state = 2;
        } else {
            elem->state = 3;
        }
    }
}
