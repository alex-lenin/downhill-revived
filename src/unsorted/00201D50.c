/* UNIT 0x00201D50 0x00201D80 */
#include "sound.h"
void func_00201D50(int value) {
    int data = value;
    func_00202048(97, 4, (void *)(&data), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
