/* UNIT 0x00202960 0x00202998 */
#include "sound.h"
void func_00202960(int x, int y) {
    int values[2];
    values[0] = x;
    values[1] = y;
    func_00202048(14, 8, (void *)(values), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
