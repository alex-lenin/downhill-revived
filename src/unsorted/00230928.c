/* UNIT 0x00230928 0x00230A70 .rodata=0x2990F0:0x299180 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge0023D4F8_28_func_0023D4F8 func_0023D4F8
#else
#define CallScalarBridge0023D4F8_28_func_0023D4F8 ScalarBridge0023D4F8_28
#endif
typedef struct Command { int type, result; union { int ints[16]; float floats[16]; struct { float a[3], b[3], c[3], d[3], e[3]; } arrays; } args; } Command;

#if defined(__mips__)
extern void func_0023D4F8(int, int, int, int);
#else

#endif

extern void func_0023D7C0(float, float, float);
#if defined(__mips__)
extern void func_0020ECD0(float, float, float, float, int, int, int);
#else
#include "scene_effects.h"
#define func_0020ECD0 SceneEffectsLegacy
#endif
extern void func_00230200(int, float, float, float, float, float);
extern void func_0022FE60(void);
int func_00230928(void *unused, Command *cmd) {
    int result = -1;
    int i;
    switch (cmd->type) {
    case 2:
        CallScalarBridge0023D4F8_28_func_0023D4F8(cmd->args.ints[0], cmd->args.ints[1], cmd->args.ints[2], cmd->args.ints[3]);
        result = cmd->result;
        break;
    case 3:
        func_0023D7C0(cmd->args.floats[0], cmd->args.floats[1], cmd->args.floats[2]);
        result = cmd->result;
        break;
    case 4:
        func_0020ECD0(cmd->args.floats[0], cmd->args.floats[1], cmd->args.floats[2], cmd->args.floats[3], cmd->args.ints[4], cmd->args.ints[5], cmd->args.ints[6]);
        result = cmd->result;
        break;
    case 5:
        for (i=0; i<3; i++) {
            func_00230200(i, cmd->args.arrays.a[i], cmd->args.arrays.b[i], cmd->args.arrays.c[i], cmd->args.arrays.d[i], cmd->args.arrays.e[i]);
        }
        func_0022FE60();
    case 7:
        result = cmd->result;
        break;
    default: break;
    }
    return result;
}
const char D_00299108[16] __attribute__((aligned(1))) = "\\RST\\%s.MDR;1";
const char D_00299118[16] __attribute__((aligned(1))) = "\\RST\\%s.MRS;1";
const char D_00299128[16] __attribute__((aligned(1))) = "\\RST\\%s.CHR;1";
const char D_00299138[16] __attribute__((aligned(1))) = "\\RST\\%s.GBL;1";
const char D_00299148[16] __attribute__((aligned(1))) = "\\RST\\%s.AIC;1";
const char D_00299158[16] __attribute__((aligned(1))) = "\\RST\\%s.RRS;1";
const char D_00299168[24] __attribute__((aligned(1))) = "\\RST\\%s.RST;1";
