/* UNIT 0x002390D0 0x00239148 */
typedef int Quad __attribute__((mode(TI)));
typedef union { Quad q; unsigned int words[4]; } Entry;
typedef struct { char pad[0x2830]; Entry *x2830; int x2834; int *x2838; char pad2[0x14]; int x2850; } Obj;
void func_002390D0(Obj *obj, Entry *entry, int value, int flags) {
    Entry copy = *entry;
    int *dest;
    copy.words[3] = (copy.words[3] & ~3) | flags;
    obj->x2830[obj->x2850] = copy;
    dest = &obj->x2838[obj->x2850];
    if (flags == 2 || flags == 0) *dest = value & 0xFFFFFF;
    else *dest = value;
}
