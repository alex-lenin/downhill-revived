/* UNIT 0x0023D618 0x0023D650 */
typedef struct Obj {
    char pad0[0x18];
    int x18;
    int x1C;
} Obj;

typedef struct Entry {
    int x0;
    Obj *x4;
    char pad8[0xC];
} Entry;

extern Entry D_0075A9D0[32];

void func_0023D618(int i, int *a, int *b)
{
    *a = D_0075A9D0[i].x4->x18;
    *b = D_0075A9D0[i].x4->x1C;
}
