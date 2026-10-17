/* UNIT 0x001FA0D0 0x001FA168 */
typedef struct Obj {
    char pad0[0x94];
    int x94;
    char pad98[0x8];
} Obj;

typedef struct Global {
    char pad0[0x1212C];
    int x1212C;
    char pad12130[0xD8];
    int x12208;
} Global;

extern Global D_004225A0;
extern int D_004B26AC[];
extern int D_00275E08[];

int func_001FA0D0(Obj *objs) {
    int result;
    int bit;
    int level;
    int i;
    int mask;

    mask = objs[D_004B26AC[0]].x94;
    level = D_004225A0.x12208 / 20;
    mask &= ~D_004225A0.x1212C;
    result = 0;
    for (i = 0; i < 11; i++) {
        bit = 1 << i;
        if ((mask & bit) && D_00275E08[i] < level) {
            result |= bit;
        }
    }
    return result;
}
