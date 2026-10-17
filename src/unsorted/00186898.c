/* UNIT 0x00186898 0x001869B0 */
#include "scalar_api.h"

#if defined(__mips__)
#define CallScalarBridge00186610_20_func_00186610 func_00186610
#else
#define CallScalarBridge00186610_20_func_00186610 ScalarBridge00186610_20
#endif
typedef struct Obj {
    char p0[0xc88];
    int xC88;
    int xC8C;
} Obj;
typedef struct Input {
    int x0;
    int x4;
    char p8[4];
    int xC;
    int x10;
    char p14[0x8d];
    char xA1[1];
} Input;


#if defined(__mips__)
extern void func_00186610(long, long);
#else

#endif

extern int func_00246220(int, int, int);
extern int func_002466A0(int, int *, int *);
extern void func_00184938(Input *, int);
extern void func_00254D68(char *, char *, int, char *);
extern int func_00186818(Obj *, char *);
extern void func_00185DE0(Input *, int);
extern void func_00186E10(Obj *, Input *);
extern char D_0028C648[];
extern char D_0029CD80[];

int func_00186898(Obj *o, Input *in) {
    char path[0x40];
    int cmd;
    int result;

    if (in->x0 == 0) {
        CallScalarBridge00186610_20_func_00186610((long)o, (long)in);
        if (o->xC88 <= 0) {
            result = func_00246220(in->x10, in->xC, o->xC8C);
            cmd = 11;
            func_002466A0(0, &cmd, &result);
            if (result < 0) {
                func_00184938(in, in->x4);
                return 1;
            }
        }
        func_00254D68(path, D_0029CD80, o->xC8C, D_0028C648);
        if (!func_00186818(o, path)) {
            func_00185DE0(in, o->xC8C);
        }
        func_00254D68(path, D_0029CD80, o->xC8C, in->xA1);
        if (!func_00186818(o, path)) {
            func_00186E10(o, in);
        }
    }
    return 1;
}
