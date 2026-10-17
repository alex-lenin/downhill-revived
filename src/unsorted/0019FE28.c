/* UNIT 0x0019FE28 0x0019FEA8 */
typedef struct Player {
    char pad0[0x7A40];
    float x7A40;
    char pad7A44[0x18C];
} Player; /* 0x7BD0 bytes */

typedef struct Game {
    Player players[10];
    char pad4D620[0x2B88];
    int count; /* 0x501A8 */
} Game;

typedef struct Obj {
    char pad0[0x8];
    int x8;
    char padC[0x10];
    int x1C[10];
} Obj;

extern Game D_002AF2C0;

void func_0019FE28(Obj *obj)
{
    int count = D_002AF2C0.count;
    int i;

    for (i = 0; i < count; i++) {
        if (obj->x1C[i] == 0) {
            obj->x1C[i] = (int)((1.0f - D_002AF2C0.players[i].x7A40) * (float)obj->x8) + obj->x8;
        }
    }
}
