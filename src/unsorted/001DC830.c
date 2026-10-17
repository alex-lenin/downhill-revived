/* UNIT 0x001DC830 0x001DC8D8 */
typedef struct Entry { char pad0[0x50]; int x50; int x54; char pad58[8]; } Entry;
typedef struct Obj { Entry entries[100]; char pad[0x25A4 - 100 * 0x60]; int x25A4[8]; int count; } Obj;
extern int func_00222BD8(int, int);
extern int D_004B2828[];
void func_001DC830(Obj *obj, int idx) {
    Entry *e, *f;
    int t;
    obj->x25A4[idx] += func_00222BD8(1, D_004B2828[0] + 1);
    if (obj->x25A4[idx] < obj->count) {
        do {
            e = &obj->entries[obj->x25A4[idx]];
            f = e;
            e = 0;
            if (f->x54 == 0) { f->x50 = idx; break; }
        } while (++obj->x25A4[idx] < obj->count);
    }
}
