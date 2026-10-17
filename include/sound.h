#ifndef DHD_SOUND_H
#define DHD_SOUND_H

#include "vector.h"

/* EE long/long long are 64/128 bits; keep those widths in native headers. */
typedef unsigned long SoundBankName __attribute__((mode(DI)));
typedef unsigned int SoundName __attribute__((mode(TI)));
typedef unsigned long SoundCallbackData __attribute__((mode(DI)));
typedef void (*SoundCompletionCallback)(int, SoundCallbackData);

typedef struct SoundCommandReturn {
    SoundCompletionCallback callback;
    int pad4;
    SoundCallbackData data;
} SoundCommandReturn;

/* 00201B68 builds the 0x30-byte named-play RPC payload. */
typedef struct SoundPlayRequest {
    union { SoundBankName name; int id; } bank;
    int volume, pan;
    SoundName name;
    short pitchModifier, pitchBend;
    int x24;
    unsigned int flags;
} SoundPlayRequest;

/* 0x40-spaced emitters initialized by 001DB4C0 and updated by 001DB510. */
typedef struct SoundEmitter {
    unsigned int handle, secondaryHandle;
    char pad8[8];
    Vector4 position;
    SoundName name;
    int nearDistance, farDistance, x38;
} SoundEmitter;

typedef char SoundBankName_size_check[sizeof(SoundBankName) == 8 ? 1 : -1];
typedef char SoundName_size_check[sizeof(SoundName) == 16 ? 1 : -1];
typedef char SoundPlayRequest_size_check[sizeof(SoundPlayRequest) == 0x30 ? 1 : -1];
typedef char SoundEmitter_size_check[sizeof(SoundEmitter) == 0x40 ? 1 : -1];
typedef char SoundCommandReturn_size_check[sizeof(SoundCommandReturn) == 0x10 ? 1 : -1];
#define DHD_SOUND_OFFSET(type, field) ((unsigned int)&((type *)0)->field)
typedef char SoundPlayRequest_name_check[DHD_SOUND_OFFSET(SoundPlayRequest, name) == 0x10 ? 1 : -1];
typedef char SoundPlayRequest_flags_check[DHD_SOUND_OFFSET(SoundPlayRequest, flags) == 0x28 ? 1 : -1];
typedef char SoundEmitter_name_check[DHD_SOUND_OFFSET(SoundEmitter, name) == 0x20 ? 1 : -1];
#undef DHD_SOUND_OFFSET

#ifdef __cplusplus
extern "C" {
#endif
void func_00201B68(int, SoundBankName, SoundName, int, int, int, int);
void func_00201BD0(int, SoundBankName, SoundName, int, int, int, int, SoundCompletionCallback, SoundCallbackData);
void func_00201E08(int, int, int, int, int, int, SoundCompletionCallback, SoundCallbackData);
void func_00201D80(int, SoundCompletionCallback, SoundCallbackData);
void func_00202048(int, int, void *, SoundCompletionCallback, SoundCallbackData);
void func_00208C50(int, SoundCallbackData);
void func_001DB4C0(SoundEmitter *, SoundName, int, int, int, int, int, int);
void func_001DB510(SoundEmitter *);
#ifdef __cplusplus
}
#endif

#endif
