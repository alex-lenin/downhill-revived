/* UNIT 0x00201AB8 0x00201AE8 */
#include "sound.h"
void func_00201AB8(int value) {
    int data = value;
    func_00202048(100, 4, (void *)(&data), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
