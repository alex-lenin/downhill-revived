/* UNIT 0x002109A0 0x00210A18 */
typedef struct {
    unsigned int mode : 6;
    unsigned int bit6 : 1;
    unsigned int x0_7 : 11;
    unsigned int x0_18 : 14;
    int x4;
    short x8;
    short xA;
    float xC;
    int x10;
    int x14;
    int x18;
    float x1C;
} State;

extern State D_00514AC0;
extern int D_0029E190;

void func_002109A0(void) {
    D_00514AC0.mode = 1;
    D_00514AC0.x0_18 = 1;
    D_00514AC0.x0_7 = 0;
    D_00514AC0.x4 = 0;
    D_00514AC0.x8 = 0;
    D_00514AC0.xA = 0;
    D_00514AC0.xC = 100.0f;
    D_00514AC0.x10 = 0;
    D_00514AC0.x14 = 0;
    D_00514AC0.x18 = 0;
    D_00514AC0.x1C = 10.0f;
    D_0029E190 = 0;
}
