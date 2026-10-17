/* UNIT 0x00139710 0x001398F8 .sdata=0x29C3C8:0x29C3D0 */
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Entry { signed char x, y, z, flags; } Entry;
typedef struct Stream { int count; Vec *cursor; char pad8[0x18]; Entry entries[1]; } Stream;
typedef struct Obj {
    char pad0[0x2bf10]; int index;
    char pad2bf14[0x316e4 - 0x2bf14]; Stream *stream;
    char pad316e8[0x31740 - 0x316e8]; Vec pos;
} Obj;

float D_0029C3C8 = 8.0f;

static inline void CopyVec(Vec *dst, Vec *src)
{
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(*dst) : "r"(src) : "$2");
}

static inline void AddVec(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvadd.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline void SubVec(Vec *dst, Vec *a, Vec *b)
{
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline float LengthVec(Vec *v)
{
    float length;
    __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
                      : "=&f"(length) : "f"(v->x), "f"(v->y), "f"(v->z));
    return length;
}

/* Rebuilds the point list from the packed stream: entries flagged 0x7F take a full
   vector from the cursor, others a 1/4-unit delta. Bit 7 marks points more than
   D_0029C3C8 away from the last marked one. */
void func_00139710(Obj *obj)
{
    Vec current, previous, delta;
    int i;

    obj->stream->cursor = (Vec *)&obj->stream->entries[obj->index];
    CopyVec(&previous, obj->stream->cursor);
    CopyVec(&obj->pos, obj->stream->cursor);
    i = 1;
    obj->stream->cursor--;
    for (; i < obj->stream->count; i++) {
        if ((obj->stream->entries[i].flags & 0x7f) == 0x7f) {
            AddVec(&current, &obj->pos, obj->stream->cursor);
            obj->stream->cursor--;
        } else {
            current.x = obj->pos.x + obj->stream->entries[i].x * 0.25f;
            current.y = obj->pos.y + obj->stream->entries[i].y * 0.25f;
            current.z = obj->pos.z + obj->stream->entries[i].z * 0.25f;
        }
        CopyVec(&obj->pos, &current);
        SubVec(&delta, &previous, &current);
        if (LengthVec(&delta) > D_0029C3C8) {
            obj->stream->entries[i].flags |= 0x80;
            CopyVec(&previous, &current);
        } else {
            obj->stream->entries[i].flags &= 0x7f;
        }
    }
}
