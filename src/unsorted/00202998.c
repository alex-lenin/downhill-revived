/* UNIT 0x00202998 0x002029D8 */
#include "sound.h"
void func_00202998(int arg0, int arg1, int arg2) {
    int args[3];
    args[0] = arg0;
    args[1] = arg1;
    args[2] = arg2;
    func_00202048(0xf, 12, (void *)(args), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}

