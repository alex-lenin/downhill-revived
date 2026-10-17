/* UNIT 0x001BBF30 0x001BC250 .sdata=0x29D544:0x29D548 */
#include "skat_queue.h"
typedef struct Object {
    char pad0[0x60C];
    int x60C;
} Object;

typedef struct Info {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    char pad14[0x8];
    int x1C;
    int x20;
    int x24;
    int x28;
    char x2C[0x8];
    char x34[0x28];
    char x5C[1];
} Info;

int D_0029D544 = 1;
extern int D_0029D548;

extern struct {
    char pad0[0x118];
    int x118;
    char pad11C[0x288 - 0x11C];
    int x288;
} D_004B25A0;

extern int func_00235590(Object *obj, int id);
extern unsigned int func_00235638(Object *obj, int id);

extern void func_00235878(Object *obj, int id);
extern void func_002337C0(Object *obj, int id, int arg);
extern void func_001DB6B0(int a, int b, int c, int *amount, int *angle, int d);
extern void *func_0023CF78(int);
extern char *func_0023CF58(int);
extern float func_00222640(void *pos);
#include "sound_math.h"
extern int func_00208E40(void *entry, int a, int b, int c);
extern int func_00238F48(void);
extern void func_001DB920(int a, int *handle, void *b);
extern void func_00202B48(int handle);
extern float func_001C23A0(void *p);
extern void func_001C22D0(void *p);
extern void func_00201DE8(unsigned int sound, int a, int amount, int angle, int extra, int b);

void func_001BBF30(Object *obj, int id, Info *info)
{
    int amount;
    int angle;
    int handle;
    int extra = 0;

    if (func_00235590(obj, id) == 4 && info->x0 == 1) {
        if (D_004B25A0.x118 == 0) {
            func_001DB6B0(info->x8, info->xC, info->x10, &amount, &angle, 0);
        } else {
            void *entry = func_0023CF78(0);
            char *pos = func_0023CF58(0) + 16;

            if (info->x4 == 0)
                angle = func_00208C60((float *)(entry), (float *)(info->x8), func_00222640(pos)) >= 181 ? 270 : 90;
            else
                angle = 0;
            amount = func_00208E40(entry, info->x8, 13, 25);
        }
        if (info->x24 == 1) {
            int elapsed = func_00238F48() - info->x28;

            /* fade out over 180 frames */
            if (elapsed < 180) {
                amount = (int)((float)amount * (1.0f - (float)elapsed * 0.0055555557f));
            } else {
                func_00235798((SkatQueueOwner *)(obj), id);
                info->x24 = 0;
            }
        }
        if (info->x4 == 1)
            angle = 0;
        if (amount > 0) {
            unsigned int sound = func_00235638(obj, id);

            if (D_004B25A0.x288 == 1) {
                func_001DB920(info->x8, &handle, info->x2C);
                func_00202B48(handle);
            }
            if (D_004B25A0.x118 != 2) {
                extra = D_0029D548 ? (int)func_001C23A0(info->x5C) : 0;
                if (D_0029D544) {
                    amount += (int)func_001C23A0(info->x34);
                    if (amount > 1024) {
                        amount = 1024;
                        func_001C22D0(info->x34);
                    } else if (amount < 0) {
                        amount = 0;
                    }
                }
            }
            /* The two do { } while (0) wrappers (macro-style) only change the
               allocator's loop-weighted reference counts: they put sound in $s3
               and id in $s4 as in retail. */
            if (sound != 0xFFFFFFFF && sound != 0)
                do { func_00201DE8(sound, 13, amount, angle, extra, 0); } while (0);
        } else {
            do { func_00235798((SkatQueueOwner *)(obj), id); } while (0);
        }
    }
    if (info->x1C && info->x20 == 0)
        func_00235798((SkatQueueOwner *)(obj), id);
    if (func_00235590(obj, id) == 4)
        info->x20 = 0;
    obj->x60C++;
    if (func_00235590(obj, id) == 1)
        func_002337C0(obj, id, 0);
    func_00235878(obj, id);
}
