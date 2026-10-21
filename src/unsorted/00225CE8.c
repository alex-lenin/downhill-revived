/* UNIT 0x00225CE8 0x00225DB0 */
#include "include_asm.h"
#ifdef NONMATCHING
/* Non-matching: $v0/$v1 and $a0/$a1 pairs swapped by local-alloc in block 0 only;
   best diff 8/50 words */
#include "gs_packet.h"
#include "particle.h"

/* Parameter block that func_00225B20 builds just before obj->x2830. */
typedef struct ParticleParams {
    float x0, x4, x8, xC, x10, x14, x18, x1C, x20, x24, x28, x2C;
    int x30, x34;
    unsigned int x38;
} ParticleParams;

extern char D_1460[];
extern int func_0023DFA0(void);

void func_00225CE8(Particle *obj)
{
    GsPacket *chain;
    ParticleParams *params;

    chain = (GsPacket *)(((unsigned int)obj + 0xC) & ~0xF);
    params = (ParticleParams *)((char *)obj->x2830 - sizeof(ParticleParams));
    ((int *)params)[-4] = obj->x2850;
    params->x4 = func_0023DFA0() ? 522352.5f : 522352.0f;
    params->x38 = 0x6C000008 | ((unsigned char)obj->x2850 << 16);
    func_002249E0((GsPacket *)((char *)params + (obj->x2850 * 16 + sizeof(ParticleParams))),
                  obj->x2824, (unsigned int)D_1460, chain, 1);
    obj->x27C0 |= 0x800;
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_00225CE8);
#endif
