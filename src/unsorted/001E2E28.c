/* UNIT 0x001E2E28 0x001E2F18 */
typedef struct Globals {
    char pad0[0x121A7];
    char x121A7[4][12];
} Globals;

typedef struct Obj {
    char pad0[0x504];
    int x504[7];
    char x520[4][13];
} Obj;

extern Globals D_004225A0;
extern void func_00194538(Globals *globals, int index);

void func_001E2E28(Obj *obj) {
    int i;
    int j;
    int id;
    int offset;
    char *name;

    for (i = 0; i < 4; i++) {
        id = i + 6;
        offset = i * 12 + 0x121A7;
        name = (char *)&D_004225A0 + offset;
        if (obj->x504[i] != 0) {
            func_00194538(&D_004225A0, id);
        }
        for (j = 0; j < 12; j++) {
            name[j] = obj->x520[i][j];
        }
        name[12] = '!';
    }
}
