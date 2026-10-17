/* UNIT 0x0021B328 0x0021B500 */
typedef struct Entry {
    int x0;
    union {
        int x4;
        struct {
            unsigned int value : 14, pad : 17, active : 1;
        };
    };
    int x8, xC;
} Entry;

typedef unsigned long Source;

typedef struct Output {
    int x0, x4;
    char *x8;
} Output;

extern Entry D_00621310[];
extern int D_0029E218;
extern int func_00218C28(int, int);
extern int func_00218B80(char *, int, int);
extern void func_0021B2E0(int, int);

static inline int Flags(int index)
{
    int addr = index * 16;
    addr += (int)D_00621310;
    return ((Entry *)addr)->x4;
}

static inline void WriteTail(unsigned long **out, unsigned long result,
                             unsigned long bits, int short3)
{
    unsigned long *destination = *out;

    if (!short3) {
        long n = (int)(bits >> 3);
        if (n <= 1) n = 1;
        result |= n << 54;
    }
    *destination = result;
}

void func_0021B328(int start, unsigned long *out, Output *buffer,
                   int count, Source *source)
{
    int index = start + 1, remaining;
    Entry *entry;
    int flags;
    unsigned long bits, result;
    int short2, short3;

    for (remaining = 0; remaining < count; remaining++) {
        entry = &D_00621310[index];
        if (!(entry->x4 & 0x80000000)) {
            entry->value = func_00218C28(index, D_0029E218);
            entry->active = 1;
            flags = entry->value;
            buffer->x8 += func_00218B80(buffer->x8, index, flags & 0x3FFF) * 16;
        } else {
            func_0021B2E0(index, D_0029E218);
        }
        index++;
    }
    bits = (*source >> 14) & 0x3F;
    {
        long n = (int)(bits >> 1);
        if (n <= 1) n = 1;
        result = (Flags(start + 1) & 0x3FFF) | (n << 14);
    }
    short2 = (unsigned int)count < 2;
    if (!short2) result |= (unsigned long)(Flags(start + 2) & 0x3FFF) << 20;
    short3 = (unsigned int)count < 3;
    if (!short2) {
        long n = (int)(bits >> 2);
        if (n <= 1) n = 1;
        result |= n << 34;
    }
    if (!short3) result |= (unsigned long)(Flags(start + 3) & 0x3FFF) << 40;
    WriteTail(&out, result, bits, short3);
}
