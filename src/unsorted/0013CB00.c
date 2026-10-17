/* UNIT 0x0013CB00 0x0013CDB0 */
#include "menu_finalize.h"
typedef struct {
    int type;
    int x4;
    int x8;
} Entry;

typedef struct {
    char pad0[0x418];
    int x418;
    char pad41c[0x8c];
    int x4a8;
    char pad4ac[0xc0];
    unsigned char x56c;
    char pad56d[3];
    int x570;
    Entry *entries;
    unsigned char indices[100];
    int count;
} Obj;

typedef struct {
    char pad[0xa0];
} Font;

extern int D_004B26AC[], D_004B2830[];
extern Font D_004C8508[];
extern char D_0029C3D8[];
extern int func_0018CC28(int);
extern int func_001F9B98(void *);
extern int func_001F9BF0(void *);
extern void func_0013D750(Obj *, int, int);
extern void func_0018E408(Obj *, void *, int);


static inline void SetFont(Obj *o, Font *fonts, int index) {
    Font *p = &fonts[index];
    func_0013D750(o, func_001F9B98(p), func_001F9BF0(p));
}

void func_0013CB00(Obj *o) {
    int kind, i;
    unsigned char enabled; /* char type keeps each `|` in its own temp */
    Entry *e;

    if (o->x56c) {
        int mode, kind, special;
        mode = D_004B26AC[0];
        kind = (func_0018CC28(o->x570) + 1) << 5;
        special = 0;
        if (kind == 224 || kind == 256 || kind == 320 || kind == 384 || kind == 416) special = 1;
        if (mode == 4 && !special) {
            SetFont(o, D_004C8508, 4);
        } else {
            SetFont(o, D_004C8508, 3);
        }
    }
    o->x4a8 = 5;
    if (o->count && o->entries) {
        kind = D_004B2830[0];
        if (o->x56c) kind = (func_0018CC28(o->x570) + 1) << 5;
        i = 0;
        o->x418 = 0;
        while (i < o->count) {
            e = &o->entries[o->indices[i]];
            enabled = !(kind == 224 || kind == 256 || kind == 320 || kind == 384 || kind == 416);
            enabled = (enabled | (kind == 224 && (unsigned)(e->type - 28) <= 1)) != 0;
            enabled = (enabled | (kind == 256 && e->type == 34)) != 0;
            enabled = (enabled | (kind == 320 && e->type == 26)) != 0;
            enabled = (enabled | (kind == 384 && e->type == 30)) != 0;
            enabled = (enabled | (kind == 416 && e->type == 31)) != 0;
            ++i;
            func_0018E408(o, D_0029C3D8, enabled);
        }
        FinalizeMenuLegacy(o);
    }
}
