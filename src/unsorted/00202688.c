/* UNIT 0x00202688 0x002026F8 */
#include "scalar_api.h"
#include "sound.h"
void func_00202688(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, ScalarCompletionCallback k, ScalarUnsigned64 l) {
    int packet[8];
    packet[0]=a; packet[1]=b; packet[2]=(e<<16)|(c&65535); packet[3]=(f<<16)|(d&65535);
    packet[4]=g; packet[5]=h; packet[6]=i; packet[7]=j;
    func_00202048(44, 32, (void *)(packet), (SoundCompletionCallback)(k), (SoundCallbackData)(l));
}
