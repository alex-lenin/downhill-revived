/* UNIT 0x001932B8 0x001933A0 */
typedef struct Hdr {
    unsigned int x0_0 : 7;
    unsigned int x0_7 : 11;
    unsigned int x0_18 : 14;
} Hdr;

typedef struct Inner {
    Hdr *x0;
} Inner;

typedef struct Obj {
    int x0;
    Inner *x4;
    char pad8[0x248];
} Obj;

extern int D_00768050[];
extern int D_0076FA78[][100];
extern Obj D_003F5690[];

static inline unsigned int GetId(Obj *obj)
{
    return obj->x4->x0->x0_18;
}

Obj *func_001932B8(unsigned int index, unsigned int id)
{
    Obj *found = 0;
    int i;
    int j;
    int count;

    for (i = 0; i < D_00768050[index]; i++) {
        if (GetId(&D_003F5690[D_0076FA78[index][i]]) == id) {
            found = &D_003F5690[D_0076FA78[index][i]];
            count = D_00768050[index]--;
            for (j = i; j < count; j++) {
                D_0076FA78[index][j] = D_0076FA78[index][j + 1];
            }
            if (found)
                break;
        }
    }
    return found;
}
