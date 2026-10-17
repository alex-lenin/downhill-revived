/* UNIT 0x001A9790 0x001A9B70 */
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
} Header;

typedef struct {
    char pad0[0x10];
    Header *x10;
    char *x14;
} Obj;

typedef struct {
    char b[0x7D0];
} Block;

int func_001A9790(Obj *obj, unsigned char *out)
{
    Rec *rec;
    int i;
    int pos;
    unsigned char count;
    unsigned short count16;
    unsigned short val;
    signed char a;
    signed char b;

    rec = (Rec *)(obj->x14 + 0x7D0);
    *(Block *)out = *(Block *)obj->x14;
    pos = 0x7D0;

    for (i = 0; i < obj->x10->count;) {
        val = rec[i].x0;
        count = 1;
        i++;
        while (i < obj->x10->count && val == rec[i].x0 && count < 255) {
            count++;
            i++;
        }
        out[pos++] = count;
        out[pos++] = val;
        out[pos++] = val >> 8;
    }

    for (i = 0; i < obj->x10->count;) {
        a = rec[i].x3;
        b = rec[i].x2;
        count = 1;
        i++;
        while (i < obj->x10->count && a == rec[i].x3 && b == rec[i].x2 && count < 255) {
            count++;
            i++;
        }
        out[pos++] = count;
        out[pos++] = a;
        out[pos++] = b;
    }

    for (i = 0; i < obj->x10->count;) {
        a = rec[i].x5;
        b = rec[i].x4;
        count = 1;
        i++;
        while (i < obj->x10->count && a == rec[i].x5 && b == rec[i].x4 && count < 255) {
            count++;
            i++;
        }
        out[pos++] = count;
        out[pos++] = a;
        out[pos++] = b;
    }

    for (i = 0; i < obj->x10->count;) {
        a = rec[i].x6;
        count16 = 1;
        i++;
        while (i < obj->x10->count && a == rec[i].x6) {
            count16++;
            i++;
        }
        out[pos++] = count16;
        out[pos++] = count16 >> 8;
        out[pos++] = a;
    }

    return pos;
}
