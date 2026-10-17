/* UNIT 0x001A9B70 0x001A9E10 */
typedef struct {
    unsigned short x0;
    signed char x2;
    signed char x3;
    signed char x4;
    signed char x5;
    signed char x6;
    signed char x7;
} Rec;

typedef struct {
    int x0;
    int count;
    int x8;
} Header;

typedef struct {
    char pad0[0x10];
    Header *x10;
    char *x14;
    char pad18[8];
    int x20;
} Obj;

typedef struct {
    char b[0x7D0];
} Block;

void func_001A9B70(Obj *obj, unsigned char *in)
{
    Rec *rec;
    int i;
    int pos;
    unsigned char c;
    unsigned short val;
    unsigned short lo;
    unsigned int hi;
    unsigned char hi2;
    signed char a;
    signed char b;
    unsigned short c16;
    signed char last;

    rec = (Rec *)(obj->x14 + 0x7D0);
    *(Block *)obj->x14 = *(Block *)in;
    pos = 0x7D0;
    obj->x20 = obj->x10->x8;

    for (i = 0; i < obj->x10->count;) {
        c = in[pos++];
        lo = in[pos++];
        hi = in[pos++];
        c16 = hi << 8;
        val = lo | c16;
        if (c != 0) {
            do {
                rec[i].x0 = val;
                i++;
                c--;
            } while (c != 0);
        }
    }

    for (i = 0; i < obj->x10->count;) {
        c = in[pos++];
        a = in[pos++];
        b = in[pos++];
        if (c != 0) {
            do {
                rec[i].x3 = a;
                rec[i].x2 = b;
                i++;
                c--;
            } while (c != 0);
        }
    }

    for (i = 0; i < obj->x10->count;) {
        c = in[pos++];
        a = in[pos++];
        b = in[pos++];
        if (c != 0) {
            do {
                rec[i].x5 = a;
                rec[i].x4 = b;
                i++;
                c--;
            } while (c != 0);
        }
    }

    for (i = 0; i < obj->x10->count;) {
        lo = in[pos++];
        hi2 = in[pos++];
        last = in[pos++];
        c16 = lo | (hi2 << 8);
        if (c16 != 0) {
            do {
                rec[i].x6 = last;
                i++;
                c16--;
            } while (c16 != 0);
        }
    }
}
