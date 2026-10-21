/* UNIT 0x0010D838 0x0010DEF8 */
#include "include_asm.h"
#ifdef NONMATCHING
/* Non-matching: fallback-branch head swaps a0/v1 for hi(D_002AF2C0) and 0x50000 (local-alloc), so li 12 / ld x440 order and one branch target differ;
   best diff 8/431 words */
typedef struct Body {
    char pad0[0x18];
    float x18;
} Body;
typedef struct Ratio {
    float x0;
    float x4;
    float x8;
} Ratio;
typedef struct Rider {
    char pad0[0x11E8];
    int x11E8;
    int x11EC;
    char pad11F0[0x124C - 0x11F0];
    int x124C;
    int x1250;
    char pad1254[0x13D4 - 0x1254];
    unsigned char x13D4;
    unsigned char x13D5;
    char pad13D6[0x2E94 - 0x13D6];
    Ratio x2E94;
    char pad2EA0[0x3FD0 - 0x2EA0];
    char x3FD0[0xC8];
    unsigned long x4098;
    char pad40A0[0x40C0 - 0x40A0];
    char x40C0[4];
    char pad40C4[0x792C - 0x40C4];
    Body *x792C;
    char pad7930[0x798C - 0x7930];
    int x798C;
    char pad7990[0x7A69 - 0x7990];
    unsigned char x7A69;
    char pad7A6A[0x7A78 - 0x7A6A];
    float x7A78;
} Rider;
typedef struct Obj {
    Rider *x0;
    int x4;
    Rider *x8;
    char padC[0x430 - 0xC];
    int x430;
    char pad434[0x440 - 0x434];
    long x440;
    long x448;
} Obj;
typedef struct Global {
    char pad0[0x501C4];
    int x501C4;
} Global;
#define BIT(n) ((long)1 << (n))
extern int D_0039E820[];
extern Global D_002AF2C0;
extern int abs(int);
extern float func_001385A8(void *);
extern void func_00155E78(void *, int);
/* AI pacing against obj->x8: clears/sets x440 bits 34/35/38 and x448 bits 2/3
   from the height gap (x792C->x18), race order (x798C), speeds (func_001385A8)
   and the race-wide level D_002AF2C0.x501C4. */
void func_0010D838(Obj *obj)
{
    int flag;
    int pdiff;
    if (obj->x430 >= 0x12D && D_0039E820[0] != 0) {
        float dz;
        float diff;
        if (obj->x8 == 0)
            return;
        dz = obj->x8->x792C->x18 - obj->x0->x792C->x18;
        if (((unsigned char)(obj->x8->x4098 >> 2) & 1) != 0) {
            if (D_002AF2C0.x501C4 < 4)
                return;
            if (!(dz < -10.0f))
                return;
            func_00155E78(obj->x0->x3FD0, 2);
            obj->x448 = (obj->x448 & ~4L) | 8L;
            return;
        }
        pdiff = obj->x8->x798C - obj->x0->x798C;
        if (D_002AF2C0.x501C4 >= 4) {
            if ((pdiff + 2) < D_002AF2C0.x501C4 && dz < 50.0f && -20.0f < dz)
                obj->x448 |= 8L;
        } else {
            if ((obj->x448 & 4L) != 0)
                obj->x448 &= ~4L;
            else {
                if ((obj->x448 & 8L) == 0)
                    return;
                obj->x448 &= ~8L;
            }
        }
        if ((obj->x448 & 8L) == 0)
            return;
        flag = 1;
        obj->x440 &= ~BIT(34);
        obj->x440 &= ~BIT(35);
        diff = func_001385A8(obj->x0->x40C0) - func_001385A8(obj->x8->x40C0);
        if (dz > 10.0f && diff > 0.1f) {
            Ratio *rp;
            float r;
            int st;
            rp = &obj->x0->x2E94;
            r = rp->x4 / rp->x8;
            if (!(r > 0.2f)) {
                if (r > 0.03f) {
                    st = obj->x0->x13D5 ? obj->x0->x1250 : obj->x0->x11EC;
                    if (st != 0)
                        goto reset_1;
                }
            } else {
reset_1:
                flag = 0;
            }
        }
        if (flag != 0) {
            if (!((pdiff + 2) < D_002AF2C0.x501C4))
                return;
            if (diff < 0.0f) {
                if (obj->x8->x7A69 != 0) {
                    obj->x440 |= BIT(34);
                    return;
                }
                if (obj->x8->x7A78 == 0.0f)
                    return;
                obj->x440 &= ~BIT(34);
                return;
            } else {
                if (obj->x8->x7A69 != 0) {
                    obj->x440 |= BIT(34);
                    return;
                }
                if (obj->x8->x7A78 != 0.0f)
                    return;
                if (!(diff < 0.1f))
                    return;
                obj->x440 |= BIT(38);
                return;
            }
        } else {
            obj->x448 &= ~8L;
            obj->x448 &= ~4L;
            obj->x440 |= BIT(34);
            obj->x440 |= BIT(35);
            return;
        }
    } else {
        float dz2;
        long initialFlags = obj->x440;
        int lim = 12;
        if (D_002AF2C0.x501C4 >= 5)
            lim = 6;
        else if (D_002AF2C0.x501C4 >= 4)
            lim = 8;
        else if (D_002AF2C0.x501C4 >= 3)
            lim = 9;
        else if (D_002AF2C0.x501C4 >= 2)
            lim = 10;
        if (!(lim < obj->x0->x798C))
            return;
        dz2 = obj->x8->x792C->x18 - obj->x0->x792C->x18;
        if (!(dz2 < 50.0f))
            return;
        if (!(-20.0f < dz2))
            return;
        obj->x440 = initialFlags & ~BIT(34);
        obj->x440 &= ~BIT(35);
        flag = 1;
        if (dz2 > 10.0f && func_001385A8(obj->x8->x40C0) < 0.5f) {
            Ratio *rp;
            float r;
            int st;
            rp = &obj->x0->x2E94;
            r = rp->x4 / rp->x8;
            if (!(r > 0.2f)) {
                if (r > 0.03f) {
                    st = obj->x0->x13D5 ? obj->x0->x1250 : obj->x0->x11EC;
                    if (st != 0)
                        goto reset_2;
                }
            } else {
reset_2:
                flag = 0;
            }
        }
        if (flag != 0) {
            int gap = obj->x8->x798C - obj->x0->x798C;
            if (D_002AF2C0.x501C4 >= 4 && abs(gap) == 1) {
                obj->x440 |= BIT(38);
            } else {
                if (func_001385A8(obj->x0->x40C0) > 0.5f) {
                    obj->x440 |= BIT(38);
                } else if (func_001385A8(obj->x0->x40C0) > 0.25f) {
                    int st;
                    if (obj->x0->x13D4 != 0)
                        st = obj->x0->x124C;
                    else
                        st = obj->x0->x11E8;
                    if (st != 0)
                        obj->x440 |= BIT(38);
                }
            }
        } else {
            obj->x440 |= BIT(34);
            obj->x440 |= BIT(35);
        }
        if (((unsigned char)(obj->x8->x4098 >> 2) & 1) != 0) {
            if (D_002AF2C0.x501C4 < 4)
                return;
            if (!(dz2 < -10.0f))
                return;
            func_00155E78(obj->x0->x3FD0, 2);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_0010D838);
#endif
