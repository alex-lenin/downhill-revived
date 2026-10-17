/* UNIT 0x00202760 0x00202820 -fno-opt-stack */
#include "sound.h"
#define TRUE 1
struct Args { int x0, x4, x8; };
extern int D_00279434[];
extern int D_0027943C[];
extern int D_00279440[];
extern volatile int D_004CD380[];
extern int func_00247D28(int, int, int);
extern int func_00202820(int);

int func_00202760(int arg0, int arg1, int arg2)
{
    struct Args args;

    if (!D_00279434[0])
        return func_00247D28(arg0, arg1, arg2);
    if (func_00202820(1) == TRUE)
        return 0;
    args.x0 = arg0;
    D_004CD380[0] = TRUE;
    D_004CD380[4] = 0;
    args.x4 = arg1;
    args.x8 = arg2;
    func_00202048(0x38, 12, (void *)(&args), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
    D_00279440[0] = 0;
    D_0027943C[0] = 1;
    return 1;
}
