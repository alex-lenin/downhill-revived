/* UNIT 0x0023AF18 0x0023AFA8 */
typedef struct Vec {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16))) Vec;

typedef struct Entry {
    Vec pos;
    float x10;
    short x14;
    short x16;
    short x18;
    char x1A;
    char x1B;
    char pad1C[4];
} Entry;

typedef struct Owner {
    char pad0[0x42];
    signed char slots[6];
} Owner;

typedef struct Obj {
    char pad0[0x664];
    signed char count;
    char pad665[0x670 - 0x665];
    Entry entries[40];
} Obj;

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

void func_0023AF18(Obj *obj, Vec *pos, Owner *owner, int x1B, int x1A)
{
    Entry *e;
    int index;
    int i;

    index = obj->count;
    if (index < 40) {
        obj->count++;
        e = &obj->entries[index];
        COPY_VECTOR(e->pos, pos);
        e->x10 = pos->w;
        e->x1B = x1B;
        e->x1A = x1A;
        e->x16 = 0;
        e->x18 = 0;
        e->x14 = 0;
        i = 0;
        while (i < 6) {
            if (owner->slots[i] == -1) {
                owner->slots[i] = index;
                break;
            }
            i++;
        }
    }
}
