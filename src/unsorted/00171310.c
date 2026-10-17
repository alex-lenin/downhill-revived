/* UNIT 0x00171310 0x001713C8 */
typedef struct Elem {
    char pad0[0x210];
} Elem;

typedef struct Obj {
    Elem x0[1];
    char pad210[0x2940 - 0x210];
    int x2940[(0x29A8 - 0x2940) / 4];
    float x29A8;
    char pad29AC[0x29BC - 0x29AC];
    int x29BC;
    char pad29C0[4];
    int x29C4;
} Obj;

extern float D_0029C8C4[];
extern char D_003AD8B0[];
extern void func_001713C8(Obj *);
extern int func_00163D40(void *, int);
extern void func_001715B8(Elem *, Obj *, int, int, int);

void func_00171310(Obj *obj)
{
    int i;

    obj->x29A8 = D_0029C8C4[0];
    obj->x29C4 = -1;
    for (i = 0; i < obj->x29BC; i++) {
        func_001713C8(obj);
        func_001715B8(&obj->x0[i], obj, obj->x2940[i], i, func_00163D40(D_003AD8B0, obj->x29C4));
    }
}
