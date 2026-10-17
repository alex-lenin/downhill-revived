/* UNIT 0x0017BE88 0x0017BEF0 */
struct Global {
    char pad0[0x288];
    int x288;
};

struct Player {
    char pad0[0x1380];
};

struct Obj {
    char pad0[0x150];
    struct Player x150[1];
};

extern struct Global D_004B25A0;
extern void func_001759E8(struct Player *player);

void func_0017BE88(struct Obj *obj) {
    int i;

    for (i = 0; i < D_004B25A0.x288; i++) {
        func_001759E8(&obj->x150[i]);
    }
}
