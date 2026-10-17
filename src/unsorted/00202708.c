/* UNIT 0x00202708 0x00202738 */
#include "sound.h"
void func_00202708(int value) {
    int x = value;
    func_00202048(0x2E, 4, (void *)(&x), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
