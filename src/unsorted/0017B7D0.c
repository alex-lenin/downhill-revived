/* UNIT 0x0017B7D0 0x0017B848 */
typedef unsigned int u128 __attribute__((mode(TI)));
struct Packet { union { u128 q; unsigned long d[2]; unsigned int w[4]; } v; };
void func_0017B7D0(void *obj, struct Packet *p, int x, int y, int width, int height) {
    p->v.d[0] = ((unsigned long)0xC00D << 46) | 0x8001;
    p->v.d[1] = 0x551;
    ++p;
    p->v.q = 0;
    ++p;
    p->v.q = 0;
    p->v.w[0] = x << 4;
    p->v.w[1] = y << 4;
    p->v.w[2] = 0xFFFF28;
    ++p;
    p->v.q = 0;
    p->v.w[0] = (x + width) << 4;
    p->v.w[1] = (y + height) << 4;
    p->v.w[2] = 0xFFFF28;
}
