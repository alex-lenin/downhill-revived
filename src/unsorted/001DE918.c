/* UNIT 0x001DE918 0x001DEDA0 */
#include "scalar_api.h"
#include "sound.h"
typedef struct Pair { int a, b; } Pair;
typedef struct Obj {
    char pad0[0x98];
    int x98;
    int x9C;
    int xA0;
    char padA4[0xB0 - 0xA4];
    float xB0;
    char padB4[0x188 - 0xB4];
    int x188;
    int x18C;
    int x190;
    int x194;
    int x198;
    char pad19C[4];
    Pair pairs[5];
    unsigned int handles[10];
    int x1F0;
    int x1F4;
    char pad1F8[8];
    int x200;
    char pad204[0xC];
    int x210;
    int x214;
    char pad218[8];
    int x220;
    int x224;
} Obj;

extern struct Global { char pad[0x5354]; char x5354[4]; } D_00398C40;
extern int func_00235590(void *, int);

#if defined(__mips__)
extern void func_00202688(int, int, int, int, int, int, int, int, int, int, void (*)(void), unsigned long);
#else

#endif

extern int func_00202738(unsigned int);
extern void func_00201D20(int);
extern void func_00201DE8(unsigned int, int, int, int, int, int);
extern int func_00238F48(void);
extern void func_001DF720(Obj *);

static inline void *get_global(struct Global *p) { return p->x5354; }

void func_001DE918(Obj *obj)
{
    void *g = get_global(&D_00398C40);
    int ready = 0;
    unsigned int h;
    float elapsed, duration;

    if (func_00235590(g, 0) == 1 && func_00235590(g, 1) == 1 && func_00235590(g, 2) == 1 &&
        func_00235590(g, 3) == 1) {
        ready = 1;
    }
    if (obj->handles[obj->x190] != 0xFFFFFFFF && obj->handles[obj->x190] != 0 && obj->x198 == 1 && obj->x220 == 0 && obj->x1F4 == 0 && ready) {
        obj->handles[obj->x18C] = 0xFFFFFFFF;
        func_00202688(obj->pairs[obj->x18C].a, obj->pairs[obj->x18C].b, 0, 0, obj->x98, 0, 1, 0, 0, 5,
                      func_00208C50, (unsigned long)(unsigned int)&obj->handles[obj->x18C]);
        obj->x220 = 1;
        obj->x188 = 0;
    } else if (obj->x220 == 1) {
        h = obj->handles[obj->x18C];
        if (h != 0xFFFFFFFF && h != 0) {
            if ((obj->x224 = func_00202738(h)) != 0) {
                func_00201D20(2);
                obj->x220 = 0;
                obj->x224 = 0;
                if (obj->x190 != 5 && obj->x190 != obj->x18C) {
                    obj->x1F4 = 1;
                    obj->x1F0 = func_00238F48();
                } else {
                    obj->x198 = 0;
                    obj->x188 = 1;
                }
                obj->x194 = obj->x190;
                obj->x190 = obj->x18C;
                obj->x214 = 1;
                obj->x200 = func_00238F48();
            }
        }
    } else if (obj->x1F4 == 1) {
        func_001DF720(obj);
    }

    if (obj->handles[obj->x190] == 0) {
        obj->x214 = 1;
        obj->x200 = func_00238F48();
        obj->handles[obj->x190] = 0xFFFFFFFF;
        func_00202688(obj->pairs[obj->x190].a, obj->pairs[obj->x18C].b, 0, 0, obj->x98, 0, 1, 0, 0, 5,
                      func_00208C50, (unsigned long)(unsigned int)&obj->handles[obj->x190]);
        obj->x220 = 1;
        obj->xA0 = obj->x98;
    } else if (obj->xA0 == obj->x98 && obj->x214 == 0) {
        obj->x214 = 1;
        obj->x200 = func_00238F48();
    } else {
        h = obj->handles[obj->x190];
        if (h != 0xFFFFFFFF && h != 0) {
            func_00201D80(h, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(&obj->handles[obj->x190]));
            if (obj->x210) {
                obj->x210 = 0;
                obj->x214 = 1;
                obj->x200 = func_00238F48() - (int)((float)(obj->xA0 - obj->x98) /
                                                    ((float)(obj->x9C - obj->x98) / (obj->xB0 * 60.0f)));
            }
            if (obj->x214) {
                elapsed = func_00238F48() - obj->x200;
                duration = obj->xB0 * 60.0f;
                if (elapsed < duration) {
                    obj->xA0 = (int)(obj->x98 + (obj->x9C - obj->x98) * (elapsed / duration));
                } else {
                    obj->x214 = 0;
                    obj->xA0 = obj->x9C;
                }
                func_00201DE8(obj->handles[obj->x190], 1, obj->xA0, 0, 0, 0);
            }
        }
    }
}
