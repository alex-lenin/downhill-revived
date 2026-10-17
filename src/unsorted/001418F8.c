/* UNIT 0x001418F8 0x00141B88 .sbss=0x29DD38:0x29DD3C */
#include "animation.h"
typedef struct { char pad0[12]; int xC; char pad10[0x94]; int xA4; char padA8[0x88]; int x130; } Obj;
typedef AnimationHandle Packet;
extern char D_002AF2C0[];
extern int D_0029DD68;
extern char D_0036B338[];
extern int D_0036ABD0[4];
extern Packet D_0036B6C0;
extern Packet *D_0036B6D0[4];
extern struct { char pad[0x290]; int x290; } D_004B25A0;
extern struct { char pad[0x5BF4]; int x5BF4; } D_00398C40;
int D_0029DD38 __attribute__((section(".sbss"))) = 0;
extern void func_001FABE8(void *, int *), func_00158840(void *, int), func_00140538(void *, int *), func_001FA878(void *);

extern int func_001FA7C0(int);
extern void func_0013FE50(int *), func_001AC328(void *, int), func_001F57D0(int, int), func_001AC620(Packet **, Packet *), func_001AC680(Packet **, float), func_001AC300(void *, int), func_002065C0(void);
static inline long packet_active(Packet **p) {
    long active = 0;
    if (*p) { if ((*p)->x0) { int result = (*p)->x4 != 0; active = result; } }
    return active;
}
static inline char *game_base(void) { return D_002AF2C0; }
void func_001418F8(Obj *obj, int *arg1, int *arg2) {
    Packet packet;
    int i;
    long active;
    func_001FABE8(game_base() + 0x78D8, arg2);
    for (i = 0; i < obj->xA4; i++) func_00158840(&D_0029DD68, obj->x130 + i);
    func_00140538(D_0036B338, arg2);
    func_001FA878(game_base() + 0x78D8);
    packet.x8 = 0;
    func_00203590(&packet, (*arg1 << 5) + 0x30, 0, 250);
    func_002038E0(&packet, 1);
    func_00203590(&packet, 0x25EE, 0, 250);
    func_00203DC8(&packet, 1);
    func_002038E0(&packet, 1);
    if ((unsigned)(*arg2 - 35) < 3) func_002036C8(&packet, 900, 0, 250, obj->x130 + 1);
    else func_00203590(&packet, 900, 0, 250);
    func_002038E0(&packet, 1);
    if (obj->xC != *arg2) D_0036ABD0[0] = func_001FA7C0(*arg2);
    func_0013FE50(D_0036ABD0);
    if (*arg2 == 31) func_001AC328(game_base() + 0x1E40, 1);
    func_001F57D0(0x25E5, D_004B25A0.x290 / 32 - 1);
    if (!D_0029DD38) { D_0036B6C0.x8 = 0; D_0029DD38 = 1; }
    D_0036B6D0[0] = 0;
    func_00203590(&D_0036B6C0, 0x210, 0, 0xFC);
    func_001AC620(D_0036B6D0, &D_0036B6C0);
    active = packet_active(D_0036B6D0);
    if ((active & 0xFF) != 0) func_001AC680(D_0036B6D0, 0.0f);
    func_001AC300(game_base() + 0x1E40, D_00398C40.x5BF4 != 0);
    func_002065C0();
}
