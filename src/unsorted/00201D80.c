/* UNIT 0x00201D80 0x00201DB0 */
#include "sound.h"
void func_00201D80(int value, SoundCompletionCallback arg1, SoundCallbackData arg2) {
    int data = value;
    func_00202048(25, 4, (void *)(&data), (SoundCompletionCallback)(arg1), (SoundCallbackData)(arg2));
}
