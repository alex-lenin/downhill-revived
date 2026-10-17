/* UNIT 0x00201948 0x00201980 */
#include "sound.h"
void func_00201948(int x, int y) {
    int values[2];
    values[0] = x;
    values[1] = y;
    func_00202048(9, 8, (void *)(values), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
