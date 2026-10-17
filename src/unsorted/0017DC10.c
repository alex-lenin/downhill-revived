/* UNIT 0x0017DC10 0x0017DC78 */
typedef struct { char pad[0x7A5C]; int x7A5C; } Owner;
typedef struct { int x0, x4, x8, xC; } Entry;
typedef struct { Owner *x0; Entry entries[62]; char pad[0xC]; unsigned char x3F0[1]; } Obj;

static inline void setValue(int *p, int value) { *p = value; }

void func_0017DC10(Obj *obj, unsigned int index, int a, int b, int c, int d) {
    int offset = index * sizeof(Entry);
    setValue((int *)((char *)obj + offset + 4), a);
    setValue((int *)((char *)obj + offset + 8), b);
    setValue((int *)((char *)obj + offset + 12), c);
    setValue((int *)((char *)obj + offset + 16), d);
    if (obj->x0->x7A5C == 1) {
        obj->x3F0[index] = (a == b && c == 0 && d == 0);
    }
}
