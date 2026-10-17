/* UNIT 0x00201CF0 0x00201D20 */
#include "sound.h"
void func_00201CF0(int value) {
    int data = value;
    func_00202048(22, 4, (void *)(&data), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
