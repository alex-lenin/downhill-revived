/* UNIT 0x001FA168 0x001FA258 */
typedef struct {
    char pad0[0x10C];
    int x10C;
    int x110;
    char pad114[0x2A0 - 0x114];
    int x2A0;
} Unk4B25A0;

typedef struct {
    char pad0[0x14];
    void *x14;
} Unk381CF0;

extern Unk4B25A0 D_004B25A0;
extern char D_004225A0[];
extern Unk381CF0 D_00381CF0[];
extern int D_00275DC8[];
extern float D_00275DE8[];

extern int func_00194430(void *a, int b, int c);
extern int func_00194E30(void *a, int b);

int func_001FA168(int arg0, int arg1, int arg2)
{
    int idx;
    int result;

    if (func_00194430(D_004225A0, arg1, D_004B25A0.x2A0) != 0) {
        int n;
        n = func_00194E30(D_004225A0 + arg2 * 0x694, D_004B25A0.x10C);
        idx = n;
        if (D_004B25A0.x110 == 1) {
            if ((D_00381CF0 + D_004B25A0.x10C)->x14 != 0) {
                idx--;
            }
        }
        result = (float)D_00275DC8[arg1] * D_00275DE8[idx];
    } else {
        result = 0;
    }
    return result;
}
