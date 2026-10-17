/* UNIT 0x00138F70 0x001391F8 */
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Record { int count; Vec *cursor; char pad[24]; signed char bytes[1][4]; } Record;
typedef struct Source { char pad[0x792C]; struct Position *pos; } Source;
typedef struct Position { char pad[16]; Vec vector; } Position;
typedef struct Obj {
    char pad[0x316E0];
    Record *record;
    char pad1[12];
    int count, previous;
    char pad2[16];
    Source *source;
    char pad3[36];
    Vec last;
} Obj;

extern void func_001391F8(Obj *, int *);

static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}

static inline void Copy(Vec *dst, Vec *src) {
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(*dst) : "r"(src) : "$2");
}

static inline void Sub(Vec *d, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*d) : "r"(a), "r"(b));
}

static inline void Add(Vec *d, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*d) : "r"(a), "r"(b));
}

void func_00138F70(Obj *obj) {
    Vec delta;
    int value;
    signed char *bytes;
    float quantized;

    obj->previous = obj->count++;
    func_001391F8(obj, &value);
    bytes = obj->record->bytes[obj->previous];

    if (obj->previous == 0) {
        Copy(obj->record->cursor, &obj->source->pos->vector);
        Copy(&obj->last, obj->record->cursor);
        obj->record->cursor->w = (float)value;
        bytes[0] = 0;
        bytes[1] = 0;
        bytes[2] = 0;
        bytes[3] = value;
        obj->record->cursor--;
    } else {
        Sub(&delta, &obj->source->pos->vector, &obj->last);
        if (Abs(delta.x) >= 32.0f || Abs(delta.y) >= 32.0f || Abs(delta.z) >= 32.0f) {
            Copy(obj->record->cursor, &delta);
            obj->record->cursor->w = (float)value;
            obj->record->cursor--;
            bytes[0] = -1;
            bytes[1] = -1;
            bytes[2] = -1;
            bytes[3] = 127;
            Add(&obj->last, &obj->last, &delta);
        } else {
            bytes[0] = (int)(delta.x * 4.0f);
            quantized = (float)bytes[0] * 0.25f;
            bytes[1] = (int)(delta.y * 4.0f);
            bytes[2] = (int)(delta.z * 4.0f);
            bytes[3] = value;
            obj->last.x += quantized;
            obj->last.y += (float)bytes[1] * 0.25f;
            obj->last.z += (float)bytes[2] * 0.25f;
        }
    }
    obj->record->count = obj->count;
}
