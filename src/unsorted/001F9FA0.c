/* UNIT 0x001F9FA0 0x001FA038 */
typedef struct Obj {
    char pad0[0x88];
    long x88;
    char pad90[0x10];
} Obj;

typedef struct Global {
    char pad0[0x12120];
    long x12120;
    char pad12128[0xE0];
    int x12208;
} Global;

extern Global D_004225A0;
extern int D_004B26AC[];
extern int D_00275EA0[];

long func_001F9FA0(Obj *objs) {
    long result;
    long bit;
    int level;
    int i;
    long mask;

    mask = objs[D_004B26AC[0]].x88;
    level = D_004225A0.x12208 / 20;
    mask &= ~D_004225A0.x12120;
    result = 0;
    for (i = 0; i < 38; i++) {
        bit = 1L << i;
        if ((mask & bit) && D_00275EA0[i] < level) {
            result |= bit;
        }
    }
    return result;
}
