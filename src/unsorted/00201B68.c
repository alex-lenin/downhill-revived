/* UNIT 0x00201B68 0x00201BD0 */
#include "sound.h"
void func_00201B68(int id, SoundBankName bankName, SoundName name, int volume, int pan, int pitchModifier, int pitchBend) {
    SoundPlayRequest p;
    unsigned int flags;
    p.flags = 0;
    if (id == 0) p.bank.name = bankName;
    else { p.bank.id = id; p.flags = 1; }
    flags = p.flags | 0x3c;
    p.name = name;
    p.volume = volume;
    p.pan = pan;
    p.pitchModifier = pitchModifier;
    p.pitchBend = pitchBend;
    p.flags = flags;
    func_00202048(0x5c, 0x30, (void *)(&p), (SoundCompletionCallback)(0), (SoundCallbackData)(0));
}
