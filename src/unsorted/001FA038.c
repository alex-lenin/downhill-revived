/* UNIT 0x001FA038 0x001FA0D0 */
typedef struct Obj {
    char pad0[0x90];
    int x90;
    char pad94[0xC];
} Obj;

typedef struct Global {
    char pad0[0x12128];
    int x12128;
    char pad1212C[0xDC];
    int x12208;
} Global;

extern Global D_004225A0;
extern int D_004B26AC[];
extern int D_00275E38[];

int func_001FA038(Obj *objs) {
    int result;
    int bit;
    int level;
    int i;
    int mask;

    mask = objs[D_004B26AC[0]].x90;
    level = D_004225A0.x12208 / 20;
    mask &= ~D_004225A0.x12128;
    result = 0;
    for (i = 0; i < 25; i++) {
        bit = 1 << i;
        if ((mask & bit) && D_00275E38[i] < level) {
            result |= bit;
        }
    }
    return result;
}
