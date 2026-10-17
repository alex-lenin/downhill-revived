/* UNIT 0x0023AFF8 0x0023B050 */
typedef struct {
    char x0;
    char x1;
    char x2;
    char x3;
    char x4;
    char x5;
} Entry;

typedef struct {
    char pad0[0x667];
    char count;
    char pad668[0xBA0 - 0x668];
    Entry entries[2];
} Obj;

int func_0023AFF8(Obj *o, int a, int b, int c, int d, int e, int f)
{
    Entry *en;

    if (o->count >= 2) {
        return -1;
    }
    en = &o->entries[o->count];
    en->x1 = b;
    en->x0 = a;
    en->x2 = c;
    en->x3 = d;
    en->x4 = e;
    en->x5 = f;
    return o->count++;
}
