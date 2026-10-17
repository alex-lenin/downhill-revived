/* UNIT 0x0023D650 0x0023D688 */
typedef struct Obj {
    int x0;
    unsigned short x4;
    unsigned short x6;
} Obj;

typedef struct Entry {
    int x0;
    Obj *x4;
    char pad8[0xC];
} Entry;

extern Entry D_0075A9D0[32];

void func_0023D650(int i, int *a, int *b)
{
    *a = D_0075A9D0[i].x4->x4;
    *b = D_0075A9D0[i].x4->x6;
}
