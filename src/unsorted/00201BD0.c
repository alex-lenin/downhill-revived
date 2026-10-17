/* UNIT 0x00201BD0 0x00201C40 */
#include "sound.h"
void func_00201BD0(int id, SoundBankName name, SoundName value, int x8, int xC, int x20, int x22, SoundCompletionCallback arg7, SoundCallbackData arg8) {
 SoundPlayRequest request;
 request.flags = 0;
 if (id == 0) request.bank.name = name;
 else { request.bank.id = id; request.flags = 1; }
 request.name = value;
 request.volume = x8;
 request.pan = xC;
 request.pitchModifier = x20;
 request.pitchBend = x22;
 request.flags |= 0x3C;
 func_00202048(0x5C, 0x30, &request, arg7, arg8);
}
