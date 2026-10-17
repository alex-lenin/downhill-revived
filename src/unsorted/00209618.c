/* UNIT 0x00209618 0x0020A5E0 .rodata=0x297CC0:0x297D80 .sdata=0x29D9D0:0x29D9D4 */
#include "particle_descriptor.h"
typedef struct Vector {
    float x, y, z, w;
} Vector __attribute__((aligned(16)));

typedef Vector Matrix[4];

typedef union Scratch {
    struct { Vector translated; Matrix saved; } group;
    struct { Matrix work; Vector negative; Matrix intermediate; } animated;
    struct { Matrix scaled; Matrix saved; } scale;
} Scratch;

typedef struct Node Node;
struct Node {
    int flags;
    int x4;
    int x8;
    Node *xC;
    float x10, x14, x18;
    Node *x1C;
    float x20, x24, x28;
    Node *x2C;
    Vector x30;
    Matrix x40;
    Matrix x80;
    unsigned long xC0;
    int xC8;
    Node *xCC;
    Node *xD0[1];
};

typedef struct Traversal {
    Vector pos;
    Matrix matrix;
    Node **nodes;
    int count;
    Node *context;
    int depth;
} Traversal;

typedef void (*Callback)(Node **, Vector *, Matrix *);

unsigned short D_0029D9D0 = 0;
extern int D_0029E170;
extern int D_0029E174;
extern Traversal D_004D5D80[];
extern Traversal D_004D5DE0[];
extern Node *D_004D9620[];

extern Node **func_002095F8(void);
extern void func_00210F20(Node *);
extern void func_00220D90(Node *);
extern void func_00221888(Matrix *, Vector *, Vector *);
extern void func_00221930(Matrix *, Matrix *, Matrix *);
extern void func_00221A60(Matrix *, Matrix *, Matrix *);
extern void func_00221AB8(Matrix *, Matrix *, Matrix *);
extern void func_00221D58(Matrix *, Matrix *);
extern void func_00221F20(Matrix *);
extern void func_00222040(Matrix *, Vector *);
extern void func_00227FD0(Node *);


#define FIELD(type, p, offset) (*(type *)((char *)(p) + (offset)))
#define CHILD(p, offset) FIELD(Node *, p, offset)
#define CHILDREN(p, offset) ((Node **)((char *)(p) + (offset)))
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
/* Sony libvu0 matrix-copy body, also used by the matched neighbouring code. */
#define COPY_MATRIX(dst, src) \
    __asm__ volatile ("lq $8, 0x0(%4)\n\tlq $9, 0x10(%4)\n\tlq $10, 0x20(%4)\n\tlq $11, 0x30(%4)\n\t" \
                      "sq $8, %0\n\tsq $9, %1\n\tsq $10, %2\n\tsq $11, %3" \
        : "=m"((dst)[0]), "=m"((dst)[1]), "=m"((dst)[2]), "=m"((dst)[3]) \
        : "r"(src) : "$8", "$9", "$10", "$11")

static __inline__ void CopyMatrix(Matrix *dst, Matrix *src)
{
    COPY_MATRIX(*dst, src);
}

static __inline__ void PushEntry(Node **nodes, int count, Vector *pos,
                                 Matrix *matrix, Node *context)
{
    Traversal *entry = &D_004D5DE0[D_0029E170];
    COPY_VECTOR(entry->pos, pos);
    COPY_MATRIX(entry->matrix, matrix);
    entry->nodes = nodes;
    entry->count = count;
    entry->context = context;
    entry->depth = D_0029E174;
    if (D_0029E170 < 150)
        D_0029E170++;
}

#define Push(pos, matrix, nodes, count, context) \
    PushEntry(nodes, count, pos, matrix, context)

static __inline__ int Pop(Traversal *state, Node **node)
{
    Traversal *entry = &D_004D5D80[D_0029E170];
    while (entry > D_004D5DE0 && entry->count == 0) {
        entry--;
        D_0029E170--;
    }
    if (entry->count != 0) {
        COPY_VECTOR(state->pos, &entry->pos);
        COPY_MATRIX(state->matrix, &entry->matrix);
        state->context = entry->context;
        D_0029E174 = entry->depth;
        *node = *entry->nodes;
        entry->count--;
        entry->nodes++;
        return 1;
    }
    D_0029E174 = 62;
    D_004D9620[63] = 0;
    return 0;
}

static __inline__ void Visit(Node *node)
{
    D_004D9620[D_0029E174--] = node;
}

/* Passing the state pointers through an inline keeps the callback's matrix
   argument out of a pseudo, which is what stops gcse from hoisting &state.matrix
   in case 31. */
static __inline__ void Report(Callback callback, Vector *pos, Matrix *matrix)
{
    callback(func_002095F8(), pos, matrix);
}

void func_00209618(Node **root, Callback callback, Vector *pos)
{
    Traversal state;
    Matrix identity;
    Scratch scratch;
    Node *node;
    int i; /* shared child index/count: one pseudo, so it gets $t4 everywhere */

    D_0029E174 = 62;
    D_004D9620[63] = 0;
    D_0029E170 = 0;
    func_00221F20(&identity);
    Push(pos, &identity, root, 1, 0);
    Visit(*root);
    while (Pop(&state, &node)) {
visit:
        Visit(node);
        switch (node->flags & 63) {
        case 41: {
            Node *current = node;
            func_00221D58((Matrix *)&current->x10, &state.matrix);
            COPY_VECTOR(FIELD(Vector, current, 0x50), &state.pos);
            if (node->flags & 0xFFFC0000)
                Report(callback, &state.pos, &state.matrix);
            break;
        }
        case 40:
            if (node->flags & 0xFFFC0000)
                Report(callback, &state.pos, &state.matrix);
            break;
        case 0:
            if (FIELD(unsigned long, node, 0) & 0xFFFC0000UL)
                Report(callback, &state.pos, &state.matrix);
            break;
        case 1: {
            Node *current = node;
            if (D_0029D9D0 & FIELD(unsigned short, current, 0xA)) {
                current->flags = (current->flags & ~63) | 9;
                break;
            }
            if (FIELD(unsigned short, current, 8) != 0) {
                if (current->flags & 0xFFFC0000)
                    Report(callback, &state.pos, &state.matrix);
                node = CHILD(current, 0x20);
                i = FIELD(unsigned short, current, 8) - 1;
                if (i != 0)
                    Push(&state.pos, &state.matrix, CHILDREN(current, 0x24), i, state.context);
                goto visit;
            }
            break;
        }
        case 2: {
            Node *current = node;
            int count = current->x4;
            if (count != 0) {
                if (current->flags & 0xFFFC0000)
                    Report(callback, &state.pos, &state.matrix);
                node = CHILD(current, 0x28);
                for (i = count - 1; i > 0; i--)
                    Push(&state.pos, &state.matrix, CHILDREN(current, 0x28 + i * 16), 1, state.context);
                goto visit;
            }
            break;
        }
        case 3: {
            Node *current = node;
            state.pos.x += current->x10;
            state.pos.y += current->x14;
            state.pos.z += current->x18;
            if (current->flags & 0xFFFC0000)
                Report(callback, &state.pos, &state.matrix);
            if (current->x8 != 0) {
                node = current->x1C;
                i = current->x8 - 1;
                if (i != 0)
                    Push(&state.pos, &state.matrix, CHILDREN(current, 0x20), i, state.context);
                goto visit;
            }
            break;
        }
        case 4: {
            Node *current = node;
            scratch.group.translated.x = state.pos.x + current->x40[0].x;
            scratch.group.translated.y = state.pos.y + current->x40[0].y;
            scratch.group.translated.z = state.pos.z + current->x40[0].z;
            func_00221888(&state.matrix, &scratch.group.translated, &state.pos);
            CopyMatrix(&scratch.group.saved, &state.matrix);
            func_00221AB8(&state.matrix, &scratch.group.saved, (Matrix *)&current->x10);
            if (FIELD(unsigned long, current, 0) & 0xFFFC0000UL)
                Report(callback, &state.pos, &state.matrix);
            if (current->x8 != 0) {
                node = CHILD(current, 0x50);
                i = current->x8 - 1;
                if (i != 0)
                    Push(&state.pos, &state.matrix, CHILDREN(current, 0x54), i, state.context);
                goto visit;
            }
            break;
        }
        case 35:
            if (node->x4 != 0)
                Push(&state.pos, &state.matrix, CHILDREN(node, 0xC), node->x4, state.context);
            break;
        case 17: {
            Node *current = node;
            Node *context = state.context;
            float **values = context ? (float **)&context->x4 : 0;
            int transform_index = FIELD(int, current, 0xC4);
            Matrix *transform = (Matrix *)context->xC + transform_index;
            short first = -1;
            unsigned char flags = 0;
            int bit = 1;
            short index;
            short rotation_index;

            index = FIELD(short, current, 0x12);
            if (index >= 0) {
                current->x20 = (*values)[index];
                first = index;
                flags |= bit;
                bit <<= 1;
            }
            index = FIELD(short, current, 0x14);
            if (index >= 0) {
                if (first < 0) first = index;
                flags |= bit;
                bit <<= 1;
                current->x24 = (*values)[index];
            }
            index = FIELD(short, current, 0x16);
            if (index >= 0) {
                if (first < 0) first = index;
                flags |= bit;
                bit <<= 1;
                current->x28 = (*values)[index];
            }
            rotation_index = FIELD(short, current, 0x18);
            if (rotation_index >= 0) {
                flags |= bit;
                if (first < 0) first = rotation_index;
                bit <<= 1;
            }
            rotation_index = FIELD(short, current, 0x1A);
            if (rotation_index >= 0) {
                flags |= bit;
                if (first < 0) first = rotation_index;
                bit <<= 1;
            }
            rotation_index = FIELD(short, current, 0x1C);
            if (rotation_index >= 0) {
                flags |= bit;
                if (first < 0) first = rotation_index;
            }
            FIELD(short, current, 0x1E) = first;
            FIELD(unsigned char, current, 0xC1) = flags;
            scratch.animated.negative.x = -current->x20;
            scratch.animated.negative.y = -current->x24;
            scratch.animated.negative.z = -current->x28;
            func_00222040(transform, &scratch.animated.negative);
            if (current->xC0 & 0x10000) {
                if (current->xC0 & 0x20000) {
                    func_00221930(&scratch.animated.intermediate, &current->x40, transform);
                    func_00221930(transform, &scratch.animated.intermediate, &current->x80);
                } else {
                    func_00221930(&scratch.animated.intermediate, &current->x40, transform);
                    COPY_MATRIX(*transform, &scratch.animated.intermediate);
                }
            } else if (current->xC0 & 0x20000) {
                COPY_MATRIX(scratch.animated.intermediate, transform);
                func_00221930(transform, &scratch.animated.intermediate, &current->x80);
            }
            __asm__ volatile (
                "lqc2 $vf11, 0(%1)\n\t"
                "lqc2 $vf12, 0(%2)\n\t"
                "vadd.xyz $vf11, $vf11, $vf12\n\t"
                "sqc2 $vf11, 0x30(%0)"
                : : "r"(transform), "r"(&(*transform)[3]), "r"(&current->x30) : "memory");
            func_00221888(&state.matrix, &(*transform)[3], &state.pos);
            CopyMatrix(&scratch.animated.work, &state.matrix);
            func_00221AB8(&state.matrix, &scratch.animated.work, transform);
            if (FIELD(unsigned long, current, 0) & 0xFFFC0000UL)
                Report(callback, &state.pos, &state.matrix);
            if (current->x4 != 0) {
                node = current->xCC;
                i = current->x4 - 1;
                if (i != 0)
                    Push(&state.pos, &state.matrix, current->xD0, i, state.context);
                goto visit;
            }
            break;
        }
        case 6: {
            Node *current = node;
            if (current->flags & 0xFFFC0000)
                Report(callback, &state.pos, &state.matrix);
            node = current->xC;
            i = FIELD(unsigned char, current, 0xB) - 1;
            if (i != 0)
                Push(&state.pos, &state.matrix, CHILDREN(current, 0x10), i, state.context);
            goto visit;
        }
        case 39: {
            Node *current = node;
            Report(callback, &state.pos, &state.matrix);
            Push(&state.pos, &state.matrix, CHILDREN(current, 0xC), 1, state.context);
            break;
        }
        case 31: {
            Node *current = node;
            Report(callback, &state.pos, &state.matrix);
            for (i = 0; i < FIELD(unsigned short, current, 0xA); i++)
                Push(&state.pos, &state.matrix, CHILDREN(current, 0xC + i * 12), 1, state.context);
            break;
        }
        case 8: {
            Node *current = node;
            node = CHILD(current, 4);
            Push(&state.pos, &state.matrix, CHILDREN(current, 8), 1, state.context);
            goto visit;
        }
        case 15:
            func_00210F20(node);
            break;
        case 45:
            func_00220D90(node);
            break;
        case 23:
            node = CHILD(node, 4);
            goto visit;
        case 25: {
            Node *current = node;
            if (FIELD(int, current, 0x28) != 0)
                Push(&state.pos, &state.matrix, CHILDREN(current, 0x2C), FIELD(int, current, 0x28), current);
            break;
        }
        case 22: {
            Node *current = node;
            if (FIELD(unsigned long, current, 0x58) & (0x8000UL << 20)) {
                unsigned long flags;
                SpawnParticleDescription(&state.pos, &state.matrix, current, 0);
                flags = FIELD(unsigned long, current, 0x58);
                flags |= 0x8000UL << 21;
                FIELD(unsigned long, current, 0x58) = flags;
            }
            break;
        }
        case 30: {
            Node *current = node;
            func_00221F20(&scratch.scale.scaled);
            scratch.scale.scaled[0].x = current->x20;
            scratch.scale.scaled[1].y = current->x24;
            scratch.scale.scaled[2].z = current->x28;
            state.pos.x += current->x10;
            state.pos.y += current->x14;
            state.pos.z += current->x18;
            state.pos.x *= 1.0f / current->x20;
            state.pos.y *= 1.0f / current->x24;
            state.pos.z *= 1.0f / current->x28;
            CopyMatrix(&scratch.scale.saved, &state.matrix);
            func_00221A60(&state.matrix, &scratch.scale.scaled, &scratch.scale.saved);
            if (FIELD(unsigned long, current, 0) & 0xFFFC0000UL)
                Report(callback, &state.pos, &state.matrix);
            if (current->x8 != 0) {
                node = current->x2C;
                i = current->x8 - 1;
                if (i > 0)
                    Push(&state.pos, &state.matrix, CHILDREN(current, 0x30), i, state.context);
                goto visit;
            }
            break;
        }
        case 10:
        case 11:
        case 42:
            if (node->flags & 0xFFFC0000)
                Report(callback, &state.pos, &state.matrix);
            func_002095F8();
            break;
        case 44: {
            Node *current = node;
            Report(callback, &state.pos, &state.matrix);
            node = CHILD(current, 4);
            goto visit;
        }
        case 5:
            func_00227FD0(node);
            break;
        default:
            break;
        }
    }
}
