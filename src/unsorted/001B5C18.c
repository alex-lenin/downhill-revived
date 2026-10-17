/* UNIT 0x001B5C18 0x001B5D78 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Packet { u128 q; unsigned long d[2]; unsigned int w[4]; } Packet;
struct Obj { char pad[0x58]; unsigned int *x58; int x5C; };
extern void func_0023D618(int, int *, int *);
extern void func_0023D650(int, int *, int *);

void func_001B5C18(struct Obj *obj, Packet *p, int index) {
    int x, y, width, height;
    int left, top;
    Packet *base;

    base = p;
    p++;
    p->d[0] = ((unsigned long)0x8000 << 45) | 0x8001;
    p->d[1] = 0xE;
    p++;
    p->d[0] = ((unsigned long)0x8000 << 23) | 0x44;
    p->d[1] = 0x42;
    p++;
    p->d[0] = ((unsigned long)0xC08D << 46) | 0x8001;
    p->d[1] = 0x551;
    func_0023D618(index, &x, &y);
    p++;
    func_0023D650(index, &width, &height);
    left = x - (width >> 1);
    top = y - (height >> 1);
    p->w[0] = 0;
    p->w[1] = 0;
    p->w[2] = 0;
    obj->x58 = &p->w[3];
    p++;
    p->w[0] = left << 4;
    p->w[1] = top << 4;
    p->w[2] = 0xFFFF28;
    p->w[3] = 0;
    p++;
    p->w[0] = (left + width) << 4;
    p->w[1] = (top + height) << 4;
    p->w[2] = 0xFFFF28;
    p->w[3] = 0;
    base->q = 0;
    base->w[0] = (p - base) | 0x60000000;
    obj->x5C = 0x28;
}
