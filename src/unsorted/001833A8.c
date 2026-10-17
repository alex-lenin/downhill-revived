/* UNIT 0x001833A8 0x001834C0 */
#include "timer.h"
typedef struct {
    char pad0[0x10C];
    int x10C;
    char pad110[0x288 - 0x110];
    int x288;
} Game;

typedef struct {
    char pad0[0x20];
    int x20;
    int pad24;
    int x28;
    int x2C;
    int x30;
    char pad34[0x100 - 0x34];
    int x100[0x64];
    int x290[1][8];
} Obj;

extern Game D_004B25A0;
extern char D_00759780[];
extern void func_00261F30();
extern void func_00218498(int, int);

void func_001833A8(Obj *obj)
{
    int i;
    int j;

    if (D_004B25A0.x10C == 0x11) {
        obj->x30 = 0;
        for (i = 0; i < obj->x28; i++) {
            func_00218498(obj->x100[i], obj->x20);
        }
        func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_00261F30), obj, 1);
        for (j = 0; j < D_004B25A0.x288; j++) {
            if (obj->x20 == 1) {
                obj->x290[j][obj->x2C] = obj->x20;
            } else {
                obj->x290[j][obj->x2C] = 50;
            }
        }
    }
}
