/* UNIT 0x002028C8 0x00202910 */
#include "sound.h"
extern int D_00279434[];
extern int func_00247FA0(void);
int func_002028C8(void) {
    int result;
    if (D_00279434[0]) {
        func_00202048(55, 0, (void *)(0), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
        result = 1;
    } else {
        result = func_00247FA0();
    }
    return result;
}

