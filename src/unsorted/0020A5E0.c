/* UNIT 0x0020A5E0 0x0020A9A0 .rodata=0x297D80:0x297E00 */
typedef struct Vec {
    float v[4];
} Vec __attribute__((aligned(16)));

typedef struct Matrix {
    Vec row[4];
} Matrix __attribute__((aligned(16)));


typedef struct Node {
    unsigned int flags;
    char pad4[0xC];
    Vec x10;
    char pad20[0x20];
    Vec x40;
} Node;

typedef struct Chain {
    char pad0[0x18];
    int state;
    char pad1C[2];
    signed char count;
    char pad1F;
    Node *nodes[1];
} Chain;

typedef struct Arena {
    int x0;
    int cursor;
    int previous;
} Arena;

typedef struct PackedFlags {
    unsigned int type : 6;
    unsigned int active : 1;
    unsigned int tex : 11;
    unsigned int group : 14;
} PackedFlags;

extern Arena D_00663B08;
extern void func_00221F20(Matrix *);
extern void func_00221A60(Matrix *, void *, Matrix *);
extern void *func_00254204(void *, int, int);

#define VTRANS(dst, src, mtx) \
    __asm__ volatile ("lqc2 $vf11, 0(%1)\n\t" \
                      "lqc2 $vf12, 0(%2)\n\t" \
                      "lqc2 $vf13, 0x10(%2)\n\t" \
                      "lqc2 $vf14, 0x20(%2)\n\t" \
                      "vmulax.xyz ACC, $vf12, $vf11x\n\t" \
                      "vmadday.xyz ACC, $vf13, $vf11y\n\t" \
                      "vmaddz.xyz $vf11, $vf14, $vf11z\n\t" \
                      "sqc2 $vf11, %0" \
                      : "=m"(dst) : "r"(src), "r"(mtx))

#define VADD_MEM(dst, a, b) \
    __asm__ volatile ("lqc2 $vf11, %1\n\t" \
                      "lqc2 $vf12, 0(%2)\n\t" \
                      "vadd.xyz $vf11, $vf11, $vf12\n\t" \
                      "sqc2 $vf11, %0" \
                      : "=m"(dst) : "m"(a), "r"(b))

#define COPY_MATRIX_TO_10(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%1)\n\t" \
                      "lq $9, 0x10(%1)\n\t" \
                      "lq $10, 0x20(%1)\n\t" \
                      "lq $11, 0x30(%1)\n\t" \
                      "sq $8, 0x10(%0)\n\t" \
                      "sq $9, 0x20(%0)\n\t" \
                      "sq $10, 0x30(%0)\n\t" \
                      "sq $11, 0x40(%0)" \
                      : : "r"(dst), "r"(src) : "$8", "$9", "$10", "$11", "memory")

void func_0020A5E0(Chain *obj, int (*filter)(unsigned int *))
{
    Vec temp;
    Matrix stack[150];
    unsigned int group;
    Matrix *matrix;
    Node *node;
    long has_pos;
    int tex;
    long has_matrix;
    int i;
    unsigned int word;
    unsigned int next_group;
    unsigned int packed_tex;
    PackedFlags *out;

    if (obj->count < 3)
        return;

    group = 1;
    matrix = &stack[0];
    func_00221F20(matrix);
    tex = 0;
    has_matrix = 0;
    has_pos = 0;
    i = 0;
    node = 0;

    for (; i < obj->count; i++) {
        node = obj->nodes[i];
        word = node->flags;
        if ((word & 0x3F) != 0x19) {
            next_group = group;
            if (word & 0xFFFC0000)
                next_group = word >> 18;
            group = next_group;
            if (word & 0x3FF80)
            {
                tex = word >> 7;
                tex &= 0x7FF;
            }
        }

        if (filter && !filter((unsigned int *)node))
            return;

        switch (node->flags & 0x3F) {
        case 3:
            has_pos = 1;
            VTRANS(temp, &node->x10, matrix);
            VADD_MEM(matrix->row[3], temp, &matrix->row[3]);
            break;

        case 4:
        {
            Matrix *next = matrix + 1;
            func_00221A60(next, &node->x10, matrix);
            has_matrix = 1;
            VTRANS(temp, &node->x40, matrix);
            VADD_MEM(*(Vec *)((char *)matrix + 0x70), temp, &matrix->row[3]);
            matrix = next;
            break;
        }

        case 9:
            obj->count = 0;
            obj->state = 1;
            return;

        case 6:
        case 17:
        case 31:
            return;

        default:
            break;
        }
    }

    if (has_matrix) {
        unsigned char *p;
        p = (unsigned char *)((D_00663B08.cursor + 15) & ~15);
        D_00663B08.cursor = (int)p + 0x64;
        func_00254204(p, 0, 0x60);
        *(unsigned long *)(p + 0) = (*(unsigned long *)(p + 0) & ~0x3FUL) | 4;
        *(int *)(p + 4) = 0;
        *(int *)(p + 8) = obj->count;
        COPY_MATRIX_TO_10(p, matrix);
        *(Node **)(p + 0x50) = obj->nodes[0];
        *(Node **)(p + 0x54) = obj->nodes[1];
        out = (PackedFlags *)p;
    } else if (has_pos) {
        unsigned char *p;
        p = (unsigned char *)((D_00663B08.cursor + 15) & ~15);
        D_00663B08.cursor = (int)p + 0x24;
        func_00254204(p, 0, 0x20);
        *(int *)(p + 4) = 0;
        *(int *)(p + 0) = (*(int *)(p + 0) & -0x40) | 3;
        *(int *)(p + 8) = obj->count;
        *(float *)(p + 0x10) = matrix->row[3].v[0];
        *(float *)(p + 0x14) = matrix->row[3].v[1];
        *(float *)(p + 0x18) = matrix->row[3].v[2];
        *(Node **)(p + 0x1C) = obj->nodes[0];
        *(Node **)(p + 0x20) = obj->nodes[1];
        out = (PackedFlags *)p;
    } else {
        unsigned char *p;
        p = (unsigned char *)((D_00663B08.cursor + 15) & ~15);
        D_00663B08.cursor = (int)p + 0x28;
        func_00254204(p, 0, 0x24);
        *(int *)(p + 4) = 0;
        *(int *)(p + 0) = (*(int *)(p + 0) & -0x40) | 1;
        *(short *)(p + 8) = (signed char)obj->count;
        *(Node **)(p + 0x20) = obj->nodes[0];
        *(Node **)(p + 0x24) = obj->nodes[1];
        out = (PackedFlags *)p;
    }

    out->group = group;
    packed_tex = tex & 0x7FF;
    out->tex = packed_tex;
    out->active = 1;
    obj->count = 2;
    obj->nodes[0] = (Node *)out;
    obj->nodes[1] = node;
}
