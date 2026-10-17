/* UNIT 0x00201CB8 0x00201CE8 */
#include "sound.h"
void func_00201CB8(int value) {
    int data = value;
    func_00202048(20, 4, (void *)(&data), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
