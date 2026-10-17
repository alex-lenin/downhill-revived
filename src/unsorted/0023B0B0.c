/* UNIT 0x0023B0B0 0x0023B0F8 */
typedef struct Entry { int x0; int x4; } Entry;
typedef struct Obj { char pad0[0x669]; signed char x669; char pad66A[0x6D2]; Entry xD3C[8]; } Obj;
int func_0023B0B0(Obj *p) {
    Entry *entry;
    if (p->x669 >= 8) return -1;
    entry = &p->xD3C[p->x669];
    entry->x0 = -1;
    entry->x4 = 0;
    return p->x669++;
}
