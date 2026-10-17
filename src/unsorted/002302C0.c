/* UNIT 0x002302C0 0x002303E8 */
extern char D_00663B58[];
extern char D_002990D0[];
extern void func_0023C1F0(void *, int *, int *, int *, int *);
extern int func_002304C0(int, int, int, int, unsigned int, int, void *, int);
extern int func_0023C2A0(void *, long, long, int, int);
extern void func_002243E8(char *);
extern void func_0023C210(void *, int);

typedef struct Obj {
    int x0, x4;
    unsigned int x8;
    int xC;
    long x10;
    long x18;
} Obj;

int func_002302C0(void *unused, Obj *obj, char *base)
{
    int a, b, c, d;
    int size, first, rest, result, available, remainder;
    unsigned int pos;
    unsigned int length;

    length = *(unsigned int *)(base + 0x50008);
    pos = obj->x8;
    size = (int)(base + length);
    rest = obj->xC;
    available = size - pos;
    first = (available >= rest) ? rest : available;
    remainder = rest - first;
    func_0023C1F0(D_00663B58, &a, &b, &c, &d);
    result = func_002304C0((a & 0x0FFFFFFF) | 0x20000000, b, (c & 0x0FFFFFFF) | 0x20000000, d,
                           pos, first, base, remainder);
    if (result > 0 && !func_0023C2A0(D_00663B58, obj->x10, obj->x18, a, result)) {
        func_002243E8(D_002990D0);
    }
    func_0023C210(D_00663B58, result);
    return result > 0;
}
