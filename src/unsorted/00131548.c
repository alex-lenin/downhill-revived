/* UNIT 0x00131548 0x00131630 */
typedef struct Entry { char pad[0x1C]; float x1C; } Entry;
typedef struct Part {
    char pad0[0x36];
    unsigned short type : 5;
    unsigned short x36_5 : 11;
    char pad38[0x38];
    float x70;
    char pad74[0xC];
} Part;
typedef struct Obj {
    char pad0[0x2E0];
    Part x2E0;
    /* second part, laid out inline: the 0xFFFF zero-extend on its type
       bitfield only appears when the containing struct is 8-aligned */
    char x360[0x34];
    unsigned short x394;
    unsigned short type : 5;
    unsigned short x396_5 : 11;
    char pad398[0x38];
    float x3D0;
    char pad3D4[0xDC];
    long x4B0;
} Obj;

extern Entry D_00768098[];
extern float func_00236088(float cur, float target, float rate, float min);

static inline Entry *GetEntry(int type) {
    if (type >= 32) return D_00768098;
    return &D_00768098[type];
}

void func_00131548(Obj *obj)
{
    Entry *a = GetEntry(obj->x2E0.type);
    Entry *b = GetEntry(obj->type);
    Part *pa = &obj->x2E0;
    Part *pb = (Part *)obj->x360;

    pa->x70 = func_00236088(pa->x70, a->x1C, 0.025f, 0.01f);
    pb->x70 = func_00236088(pb->x70, b->x1C, 0.025f, 0.01f);
}
