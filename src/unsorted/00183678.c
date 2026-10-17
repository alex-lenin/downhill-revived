/* UNIT 0x00183678 0x00183770 */
typedef struct Obj {
    int x0[4];
    int x10[4];
    int mode;
    char pad24[0x2C - 0x24];
    int x2C;
    int x30;
    char pad34[0x1C8 - 0x34];
    int x1C8[50];
    int x290[4][8];
} Obj;

typedef struct Game {
    char pad0[0x288];
    int playerCount;
} Game;

extern Game D_004B25A0;
extern char D_004B1F40[];
extern void func_001B45F8(void *, int, int);

void func_00183678(Obj *obj, int id, int player) {
    int idx;

    if (obj->x10[player] == 0) {
        idx = id - 0x33F4;
        if (obj->x0[player] == 0) {
            obj->x10[player] = 1;
            if (obj->mode == 1) {
                if (idx > 0) {
                    obj->x290[player][obj->x2C] = obj->x1C8[idx] / 100 + 1;
                }
            } else if (idx > 0) {
                obj->x290[player][obj->x2C] = obj->x1C8[idx];
            }
        }
        obj->x30++;
    }
    if (obj->x30 >= D_004B25A0.playerCount) {
        func_001B45F8(D_004B1F40, 1, 300);
    }
}
