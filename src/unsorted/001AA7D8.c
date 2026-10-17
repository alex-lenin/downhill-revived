/* UNIT 0x001AA7D8 0x001AAAE8 .rodata=0x28E8B0:0x28E8E0 */
/* Reads a parameter block: a list of float tokens (key, value...) */

/* status: 0 = more tokens, 1 = all tokens consumed, 2 = error */
typedef struct { int index; int count; float *values; int status; } Sequence;

typedef struct Header {
    char pad0[0xA];
    short count;
    float values[1];
} Header;

typedef struct Params {
    float x0, x4, x8, xC, x10, x14, x18;
    int x1C;            /* number of 4-float entries at x28 */
    int x20;            /* number of 3-float entries at x2C */
    int x24;
    float *x28;
    float *x2C;
    float x30, x34, x38;
} Params;

extern float D_002702C0[];
extern void func_001AA770(Sequence *seq, float *out);

int func_001AA7D8(Header *header, Params *out)
{
    Sequence seq;
    float key_f, count_f4, count_f3;
    int key;
    int next4, count4, next3, count3;
    float *ptr;
    int status;
    long done; /* the retail daddu before the loop test is this int -> long copy */

    out->x0 = 1.0f;
    out->x4 = 0.0f;
    out->x8 = 0.5f;
    out->xC = 0.25f;
    out->x14 = out->x10 = 0.995f;
    out->x18 = 2.0f;
    out->x1C = 1;
    out->x20 = 0;
    out->x28 = D_002702C0;
    out->x2C = 0;
    out->x30 = out->x34 = out->x38 = out->x4;

    seq.index = 0;
    seq.count = header->count;
    seq.values = header->values;
    seq.status = 0;
    if (seq.index > seq.count) seq.status = 2;
    else if (seq.index == seq.count) seq.status = 1;

    while (status = seq.status, done = status, status == 0) {
        func_001AA770(&seq, &key_f);
        key = (int)key_f;
        if ((float)key != key_f) seq.status = 2;
        switch (key) {
        case 1: func_001AA770(&seq, &out->x0); break;
        case 2: func_001AA770(&seq, &out->x4); break;
        case 3: func_001AA770(&seq, &out->x8); break;
        case 4: func_001AA770(&seq, &out->xC); break;
        case 5: func_001AA770(&seq, &out->x14); break;
        case 6: func_001AA770(&seq, &out->x10); break;
        case 7: func_001AA770(&seq, &out->x18); break;
        case 8:
            func_001AA770(&seq, &count_f4);
            out->x1C = (int)count_f4;
            if ((float)out->x1C != count_f4) seq.status = 2;
            next4 = seq.index + out->x1C * 4;
            out->x28 = seq.values + seq.index;
            seq.index = next4;
            count4 = seq.count;
            if (count4 < next4) goto error;
            if (next4 == count4 && seq.status != 2) seq.status = 1;
            break;
        case 9:
            func_001AA770(&seq, &count_f3);
            out->x20 = (int)count_f3;
            if ((float)out->x20 != count_f3) seq.status = 2;
            ptr = seq.values + seq.index;
            next3 = seq.index + out->x20 * 3;
            seq.index = next3;
            count3 = seq.count;
            out->x2C = ptr;
            if (count3 < next3) {
error:
                seq.status = 2;
            } else if (next3 == count3 && seq.status != 2) seq.status = 1;
            break;
        case 10:
            func_001AA770(&seq, &out->x30);
            func_001AA770(&seq, &out->x34);
            func_001AA770(&seq, &out->x38);
            break;
        default:
            return 0;
        }
    }
    return done == 1;
}
