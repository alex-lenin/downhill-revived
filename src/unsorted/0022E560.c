/* UNIT 0x0022E560 0x0022E9A0 */
#include "gs_packet.h"
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Matrix {
    Vec4 row[4];
} Matrix;

typedef struct Entry {
    int x0, x4, x8, xC;
} Entry;

typedef struct Obj {
    char pad0[0x2810];
    unsigned int x2810;
    int x2814;
    char pad2818[0x282C - 0x2818];
    int x282C;
    Vec4 *x2830;
    Vec4 *x2834;
} Obj;

extern Entry D_0067DF90[];
extern float D_0027EB10[];
extern int func_00219EE8(int, Entry *, int *);

extern Matrix *func_0023CF58(int);
extern void func_00221888(Matrix *, Vec4 *, Vec4 *);

#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
#define COPY_VEC_M(dst, src) \
    __asm__ volatile ("lq $2, %1\n\tsq $2, %0" : "=m"(dst) : "m"(src) : "$2")
#define COPY_MATRIX(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\t" \
                      "sq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
                      : "=m"((dst).row[0]), "=m"((dst).row[1]), "=m"((dst).row[2]), "=m"((dst).row[3]) \
                      : "r"(src) : "$8", "$9", "$10", "$11")

int *func_0022E560(int *p, Obj *obj, int **head, Matrix *mtx, int idx)
{
    unsigned int flags = obj->x2810;
    unsigned int mode = flags & 0xFFFFFF8F;
    Entry *entry = &D_0067DF90[obj->x2814];
    int n;

    if (mode - 0x8007 < 2) {
        if (((Entry *)&obj->x2830[idx])->xC != 0) {
            p += func_00219EE8(entry->x8, entry, p) * 4;
            *head = p;
            p[0] = 0x5000000A;
            p[1] = ((unsigned int)obj + 12) & 0xFFFFFFF0;
            p[2] = 0x11000000;
            p[3] = 0x50000002;
            (int *)func_00224660((GsPacket *)(p + 4), 0, 14, 1, 0, 0, 0, 1, 1);
            p[8] = entry->x0;
            p[9] = entry->x4;
            p[10] = 6;
            p[11] = 0;
            p[12] = 0;
            p[13] = 0;
            p[14] = 0;
            if ((obj->x2810 & 0xFFFFFF8F) == 0x8008) {
                Vec4 v;
                **head = 0x5000000B;
                p[15] = 0x6C08000E;
                COPY_MATRIX(*(Matrix *)(p + 16), mtx);
                COPY_VEC_M(*(Vec4 *)(p + 32), obj->x2830[idx]);
                COPY_VEC_M(*(Vec4 *)(p + 36), obj->x2834[idx]);
                COPY_VEC(*(Vec4 *)(p + 40), idx * 16 + (int)obj->x2834 + 0x40);
                n = obj->x282C;
                v = (Vec4){ D_0027EB10[n], D_0027EB10[n + 4], D_0027EB10[n + 8] };
                func_00221888(func_0023CF58(idx), &v, &v);
                ((float *)p)[44] = v.x;
                ((float *)p)[46] = v.y;
                ((float *)p)[45] = -v.z;
                if (n != 0) {
                    ((float *)p)[47] = n * 16 + 0x38;
                } else {
                    ((float *)p)[47] = 9.0f;
                }
                return p + 48;
            }
            p[15] = 0x6C070012;
            COPY_VEC_M(*(Vec4 *)(p + 16), obj->x2830[idx]);
            COPY_VEC_M(*(Vec4 *)(p + 20), obj->x2834[idx]);
            COPY_VEC(*(Vec4 *)(p + 24), idx * 16 + (int)obj->x2834 + 0x40);
            COPY_MATRIX(*(Matrix *)(p + 28), mtx);
            return p + 44;
        }
        return p;
    }
    if (flags & 0x8000) {
        p += func_00219EE8(entry->x8, entry, p) * 4;
        *head = p;
        p[0] = 0x50000002;
        p[1] = ((unsigned int)obj + 12) & 0xFFFFFFF0;
        p[2] = 0x11000000;
        p[3] = 0x50000002;
        (int *)func_00224660((GsPacket *)(p + 4), 0, 14, 1, 0, 0, 0, 1, 1);
        p[8] = entry->x0;
        p[9] = entry->x4;
        p[10] = 6;
        p[11] = 0;
        return p + 12;
    }
    if (mode == 1) {
        *head = p;
        p[0] = 0x50000004;
        p[1] = ((unsigned int)obj + 12) & 0xFFFFFFF0;
        p[2] = 0x11000000;
        p[3] = 0x6C04021A;
        COPY_MATRIX(*(Matrix *)(p + 4), mtx);
        return p + 20;
    } else {
        *head = p;
        p[0] = 0x50000000;
        p[1] = ((unsigned int)obj + 12) & 0xFFFFFFF0;
        p[3] = 0x11000000;
        p[2] = 0;
        return p + 4;
    }
}
