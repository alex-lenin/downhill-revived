/* UNIT 0x002025A0 0x002025D0 */
#include "sound.h"
void func_002025A0(int value) {
    int data = value;
    func_00202048(47, 4, (void *)(&data), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
