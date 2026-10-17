/* UNIT 0x001832B0 0x00183368 */
typedef struct Obj {
    int x0[4];
    int x10[4];
    char pad20[0x4];
    int count;
    char pad28[0x4];
    int x2C;
    int x30;
    char pad34[0x4];
    int x38[8];
} Obj;

typedef struct Game {
    char pad0[0x288];
    int playerCount;
} Game;

extern Game D_004B25A0;
extern void func_00218498(int, int);

void func_001832B0(Obj *obj) {
    int i;
    int j;

    for (i = 1; i < obj->count; i++) {
        func_00218498(obj->x38[i], 0);
    }
    obj->x2C = 1;
    obj->x30 = 0;
    for (j = 0; j < D_004B25A0.playerCount; j++) {
        obj->x0[j] = 0;
        obj->x10[j] = 0;
    }
}
