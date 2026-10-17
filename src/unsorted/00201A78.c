/* UNIT 0x00201A78 0x00201AB8 */
#include "sound.h"
void func_00201A78(int arg0, int arg1, int arg2) {
    int args[3];
    args[0] = arg0;
    args[1] = arg1;
    args[2] = arg2;
    func_00202048(0x4e, 12, (void *)(args), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
