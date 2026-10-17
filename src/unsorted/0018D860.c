/* UNIT 0x0018D860 0x0018D898 */
typedef struct {
    int *x0;
} Table;

typedef struct {
    char pad0[0x4];
    short x4;
    char pad6[0x2];
    int x8;
    Table *xC;
} Obj;

int func_0018D860(Obj *obj)
{
    short idx;

    if (obj->x8 != 0) {
        return obj->x8;
    }
    idx = obj->x4;
    if (idx < 0) {
        return 0;
    }
    return obj->xC->x0[idx];
}
