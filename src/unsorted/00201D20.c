/* UNIT 0x00201D20 0x00201D50 */
#include "sound.h"
void func_00201D20(int value) {
    int data = value;
    func_00202048(23, 4, (void *)(&data), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
