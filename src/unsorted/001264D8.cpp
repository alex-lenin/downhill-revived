/* UNIT 0x001264D8 0x00126898 .rodata=0x288E20:0x288E90 */
#include "rider_ai.h"
#include "rider_event.h"
#include "rider_queries.h"
#if defined(__mips__)
#define CallShared0011B268_0 func_0011B268
#else
#define CallShared0011B268_0 Forward0011B268_2
#endif
struct Vec { float x, y, z, w; };
struct Transform { unsigned long flags; char pad8[8]; Vec x10; };
class Part {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual Transform *GetTransform();
};
struct Rider { char pad0[0x234]; char x234[0x18]; unsigned char x24C; };
struct Obj {
    char pad0[8];
    int x8;
    char padC[4];
    int x10;
    char pad14[0x54];
    char x68[0xC8];
    Part *x130;
    char pad134[0x10];
    int x144;
};
extern "C" {
extern char D_002AF2C0[];
extern float D_0029C220[];
int func_0011C0D0(void *, Vec *, int, float);
int func_0011B268(void *, Vec *, float *, Vec *, float, float);
void func_00123760(Obj *, int);
void func_00126B20(Obj *, int);

void func_00126898(Obj *);
void func_001238B8(Obj *);
void func_00123788(Obj *, Vec *);
void func_00123AD0(Obj *, Vec *, int);

void func_00123F60(Obj *, int);
float func_001ACBA0(void *);
float func_00235EA8(float, float, float, float);
void func_001ACAE0(void *, float);
}

static inline void Decay(void *anim)
{
    func_001ACAE0(anim, func_00235EA8(func_001ACBA0(anim), 0.5f, 0.1f, 0.0001f));
}

static inline int IsVisible(Obj *obj)
{
    if (obj->x10 != 0) {
        return 1;
    }
    return func_0011C0D0(D_002AF2C0, &obj->x130->GetTransform()->x10, 0, 700.0f);
}

extern "C" void func_001264D8(Obj *obj)
{
    Vec pos;
    float dist;

    if (!IsVisible(obj)) {
        if (obj->x8 != 0) {
            func_00126B20(obj, 0);
            ((Rider *)obj->x130)->x24C = 1;
            obj->x144 = 0;
        }
        return;
    }
    ((Rider *)obj->x130)->x24C = 0;
    if (obj->x8 == 8) {
        obj->x130->GetTransform()->flags &= ~0x0200000000000000UL;
    } else {
        obj->x130->GetTransform()->flags |= 0x0200000000000000UL;
    }
    dist = D_0029C220[0];
    func_00123760(obj, CallShared0011B268_0(D_002AF2C0, &obj->x130->GetTransform()->x10, &dist, &pos, 70.0f, -20.0f));
    if (obj->x144 != 0) {
        if (dist < 9.0f) {
            if (obj->x8 != 5) {
                func_00126B20(obj, 5);
                SubmitRiderEvent(((Rider *)obj->x130)->x234, obj->x130);
            }
            goto done;
        } else if (dist < 144.0f) {
            if (obj->x8 != 4) {
                func_00126B20(obj, 4);
            }
            goto done;
        }
    }
    if ((unsigned int)(obj->x8 - 4) < 2) {
        if (obj->x144 != 0) {
            if (576.0f < dist) {
                func_00126B20(obj, 2);
            }
        } else {
            func_00126B20(obj, 2);
        }
    }
done:
    switch (obj->x8) {
    case 3:
        func_00126898(obj);
        func_001238B8(obj);
        break;
    case 2:
        if (dist < 40000.0f) {
            func_00123788(obj, &pos);
        }
    case 0:
    case 1:
        Decay(obj->x68);
        func_00123F60(obj, 0);
        break;
    case 4:
        func_00123AD0(obj, &pos, 0);
        func_001238B8(obj);
        break;
    case 5:
        UpdateRiderAiLegacy(obj, &pos);
        Decay(obj->x68);
        break;
    case 8:
        Decay(obj->x68);
        break;
    }
}

/* used by func_00127CC8; it follows the jump table in the same .rodata, so the table is not padded to 16 */
extern const char D_00288E48[0x48] = "Group %d, point %d failed to find a ped of type %d (%d available)\n";
