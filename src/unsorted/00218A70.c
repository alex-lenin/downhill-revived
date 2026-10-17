/* UNIT 0x00218A70 0x00218B80 */
typedef struct Entry {
    int x0;
    unsigned int index : 14;
    unsigned int pad : 11;
    unsigned int count : 3;
    unsigned int pad2 : 3;
    unsigned int active : 1;
    int x8;
    int xC;
} Entry;

typedef struct Ref {
    unsigned short index : 14;
    unsigned short flags : 2;
} Ref;

typedef struct Quad {
    int x[4];
} Quad;

typedef struct Ctx {
    char pad0[8];
    Quad *x8;
} Ctx;

extern Entry D_00621310[];
extern char D_002983E0[];

extern int func_00218C28(int);
extern int func_00218B80(Quad *, int, int);
extern void func_002189F8(char *, int, int);
extern void func_0021B328(int, long *, Ctx *, int, Ref *);

void func_00218A70(unsigned int id, Ref *ref, Ctx *ctx, long *out)
{
    Entry *entry = &D_00621310[id];

    entry->index = func_00218C28(id);
    ctx->x8 += func_00218B80(ctx->x8, id, entry->index);
    entry->active = 1;
    ref->index = 0;
    ref->index |= entry->index;
    func_002189F8(D_002983E0, id, entry->count);
    if (entry->count != 0) {
        func_0021B328(id, out, ctx, entry->count, ref);
    } else {
        *out = 0;
    }
}
