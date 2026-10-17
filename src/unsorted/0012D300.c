/* UNIT 0x0012D300 0x0012D360 */
typedef struct Entry {
    char pad0[0x2BC];
    int x2BC;
    char pad2C0[0x380 - 0x2C0];
} Entry;

typedef struct Obj {
    char pad0[0x7908];
    int x7908;
    char pad790C[0x7984 - 0x790C];
    int x7984;
    int x7988;
} Obj;

extern Entry D_0037EA60[];

int func_0012D300(Obj *p)
{
    int empty;

    if (p->x7908 >= 0x23) {
        if (p->x7908 < 0x26) {
            return -16;
        }
    }
    if (p->x7984 == 0) {
        return 0;
    }
    empty = (D_0037EA60 + p->x7988)->x2BC == 0;
    return empty * 24;
}
