/* UNIT 0x002019A8 0x00201A30 */
#include "sound.h"
typedef struct {
    int x0;
    unsigned int x4, x8, xc, x10, x14;
} Payload;
typedef struct {
    int x0;
    Payload x4;
} Request;
void func_002019A8(int a, Payload *p) {
    Request r;
    Payload *out=&r.x4;
    r.x0=a;
    if (p) *out=*p;
    else {
        out->x0=-1;
        out->x4=0xffffffffU;
    } func_00202048(0x60, 0x1c, (void *)(&r), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
