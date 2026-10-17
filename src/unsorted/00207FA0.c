/* UNIT 0x00207FA0 0x00208158 .sdata=0x29D9B0:0x29D9C0 */
#include "disc_path.h"
#include "include_asm.h"

typedef struct {
    char pad[0x20];
    int x20;
    int x24;
    int x28;
    int x2c;
} Obj;

/* Per-state callback: called with (char *)obj + offset when enabled != 0. */
struct Handler {
    short offset;
    unsigned short enabled;
    void (*fn)(void *);
};

struct CdRMode {
    unsigned char trycount;
    unsigned char spindlctrl;
    unsigned char datapattern;
    unsigned char pad;
};

struct Pool {
    int x0;
    int x4;
};

extern struct Handler D_002799B8[];
Obj *D_0029D9B0 = 0;
struct Pool *D_0029D9B4 = 0;

int func_00207FA0(Obj *obj)
{
    struct Handler h;

    if (D_0029D9B0 == obj) {
        h = D_002799B8[obj->x20];
        if (h.enabled) {
            h.fn((char *)obj + h.offset);
        }
    }
    return obj->x20;
}



struct CdFile {
    unsigned int lsn;
    unsigned int size;
    char name[16];
    unsigned char date[8];
};

extern struct CdFile D_004D02F0;
extern char D_0029D9B8[8];
extern struct CdRMode D_0029E168;

extern int func_0020E520(struct CdFile *file, Obj *obj);
void func_002080F8(Obj *obj, int value);

int func_00208008(Obj *obj, int arg1, int arg2, struct Pool *pool, char *ext)
{
    int aligned;
    int size;
    register char *name;

    if (D_0029D9B0 != 0) {
        return 0;
    }
    name = D_0029D9B8;
    name = ext ? ext : name;
    BuildDiscPathWithMode((char *)(obj), (const char *)(arg2), (const char *)(arg1), (const char *)(name), (char *)0);
    D_004D02F0.lsn = 0;
    D_0029E168.trycount = 20;
    D_0029E168.spindlctrl = 1;
    D_004D02F0.size = 0;
    D_004D02F0.name[0] = 0;
    D_004D02F0.date[0] = 0;
    D_0029E168.datapattern = 0;
    if (func_0020E520(&D_004D02F0, obj) == 0) {
        return 0;
    }
    D_0029D9B0 = obj;
    D_0029D9B4 = pool;
    obj->x20 = 2;
    size = D_004D02F0.size;
    obj->x24 = size;
    if (pool != 0) {
        aligned = (pool->x4 + 63) & ~63;
        pool->x4 = aligned + size;
        func_002080F8(obj, aligned);
    }
    return 1;
}

void func_002080F8(Obj *obj, int value) {
    if (D_0029D9B0 == obj) {
        obj->x28 = value;
        obj->x20 = 3;
        obj->x2c = 0;
    }
}

void func_00208118(Obj *obj) {
    Obj *current = D_0029D9B0;
    if (current != obj) return;
    if (current->x20 == 4) goto state4;
    if (current->x20 != 3) return;
    D_0029D9B0 = 0;
    current->x20 = 8;
    return;
state4:
    current->x20 = 6;
}

char D_0029D9B8[8] = "bdd";
