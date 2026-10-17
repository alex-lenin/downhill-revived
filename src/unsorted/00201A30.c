/* UNIT 0x00201A30 0x00201A60 */
#include "sound.h"
void func_00201A30(int value) {
    int data = value;
    func_00202048(11, 4, (void *)(&data), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
