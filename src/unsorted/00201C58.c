/* UNIT 0x00201C58 0x00201C88 */
#include "sound.h"
void func_00201C58(int value) {
    int data = value;
    func_00202048(21, 4, (void *)(&data), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
