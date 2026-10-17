/* UNIT 0x00201E08 0x00201E50 */
#include "sound.h"
void func_00201E08(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, SoundCompletionCallback arg6, SoundCallbackData arg7) {
    int data[6];
    data[0] = arg0;
    data[1] = arg1;
    data[2] = arg2;
    data[3] = arg3;
    data[4] = arg4;
    data[5] = arg5;
    func_00202048(0x21, 0x18, data, arg6, arg7);
}
