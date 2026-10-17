/* UNIT 0x002013F8 0x00201628 -fno-opt-stack */
#include "sound.h"
typedef SoundCompletionCallback Callback;

typedef SoundCommandReturn Entry;

extern int D_00279428[], D_0027943C[], D_00279440[], D_00279444[];
extern int D_00279468[], D_0027946C[], D_00279470[], D_004C8984[];
extern unsigned int D_004CD400[];
extern void (*D_00279438[])(int);
extern Callback D_00279488[];
extern SoundCallbackData D_00279490[];
extern int *D_00279448[];
extern Entry *D_00279458[];
extern int *D_00279460[];
extern Entry D_00279478;

extern int func_00201640(void);
extern void func_00202388(int *);
extern void func_00202820(int);
extern void func_0025A960(int);

int func_002013F8(void)
{
    int idx, i, result;
    Entry *e;
    Callback cb;
    SoundCallbackData arg;

    if (D_00279428[0] && func_00201640()) {
        if (D_00279444[0]) {
            if (D_00279488[0])
                D_00279488[0](D_004C8984[0], D_00279490[0]);
            D_00279488[0] = 0;
            D_00279444[0] = 0;
        } else {
            idx = D_00279468[0] != 1;
            i = 0;
            if (*D_00279448[idx] > 0) {
                do {
                    e = (Entry *)((i << 4) + (int)D_00279458[idx]);
                    if (e->callback)
                        e->callback(D_00279460[idx][i + 1], e->data);
                    i++;
                } while (i < *D_00279448[idx]);
            }
        }
    }
    if (D_00279470[0]) {
        func_0025A960(0);
        if (D_004CD400[0] != 0xFFFFFFFFU) {
            cb = D_00279478.callback;
            if (cb) {
                arg = D_00279478.data;
                D_00279478.callback = 0;
                D_00279478.data = 0;
                cb(D_004CD400[0], arg);
            }
            D_004CD400[0] = 0;
            D_00279470[0] = 0;
        }
    }
    if (!D_00279428[0]) {
        int *buf = D_00279448[D_00279468[0]];
        if (*buf && !D_0027946C[0])
            func_00202388(buf);
    }
    if (D_0027943C[0]) {
        func_00202820(1);
        if (D_00279440[0]) {
            D_0027943C[0] = 0;
            D_00279440[0] = 0;
            if (D_00279438[0])
                D_00279438[0](1);
        }
    }
    result = 0;
    if (D_00279428[0] || D_00279470[0])
        result = 1;
    return result;
}
