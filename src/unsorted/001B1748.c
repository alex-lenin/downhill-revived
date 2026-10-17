/* UNIT 0x001B1748 0x001B1C08 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Mat {
    u128 q[4];
} Mat __attribute__((aligned(16)));

typedef struct Bone {
    char pad0[0x12];
    short x12, x14, x16, x18;
    char pad1A[0x40 - 0x1A];
    Mat x40, x80;
} Bone;

typedef struct Model {
    char pad0[0x20];
    struct { unsigned short x0, count; } *info;
} Model;

typedef struct Anim {
    char pad0[8];
    struct Root *root;
    float *values;
    unsigned char *bits;
} Anim;

typedef struct Parent {
    char pad0[0x16D0];
    char *x16D0;
} Parent;

typedef struct Root {
    char pad0[0xA0];
    Vec points[3];
    Bone *bones[3];
    char padDC[4];
    int xE0, xE4;
    char padE8[8];
    Vec direction;
    unsigned char x100;
    char pad101[0x10C - 0x101];
    Parent *parent;
} Root;

extern int func_001577B0(void *);
extern int func_001B1F68(void *, Root *);
extern void func_001A7AA0(void *, Anim *, Model *, int);
extern void func_001B16C8(Mat *, Vec *, Vec *, Vec *);
extern void func_00221AB8(Mat *, Mat *, Mat *);
extern void func_00221D58(Mat *, Mat *);
extern void func_00221930(Mat *, Mat *, Mat *);
extern void func_002224E0(Mat *, Vec *);
extern void func_00254204(unsigned char *, int, int);
extern Mat D_00769830[];

static inline void SUBN(Vec *dst, Vec *a, Vec *b) {
    __asm__ volatile ("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0"
                      : "=m"(*dst) : "r"(a), "r"(b));
}

static inline float VLength(Vec *src) {
    float len;
    float x = src->x, y = src->y, z = src->z;
    __asm__ volatile ("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0"
                      : "=&f"(len) : "f"(x), "f"(y), "f"(z));
    return len;
}

#define STORE(field, val) \
    anim->values[bone->field] = (val); \
    { unsigned int bit = bone->field; unsigned char *flags = anim->bits + (bit >> 3); *flags |= 1 << (bit & 7); }

void func_001B1748(Anim *anim, Model *model) {
    Vec rot;
    Mat current, alternative, temp, result;
    Vec delta;
    Root *root;
    Bone *bone;
    Mat *previous, *active, *next;
    int i;
    int j, alt, curr, next_i;
    Vec *p, *q;

    func_00254204(anim->bits, 0, model->info->count >= 5);
    root = anim->root;
    if (func_001577B0(root->parent->x16D0 + 0x4EE0)) {
        int index = func_001B1F68((char *)root->parent + 0x1CB0, root);
        func_001A7AA0(root->parent->x16D0 + 0x4EF0, anim, model, index);
        return;
    }

    previous = &D_00769830[root->xE0];
    active = &current;
    next = &alternative;
    for (i = 0; i < 2;) {
        curr = i;
        next_i = -~curr;
        alt = curr - 2;
        j = curr + 1;
        if (j >= 3) j = alt;
        p = &root->points[curr];
        q = &root->points[j];
        func_001B16C8(active, p, q, &root->direction);
        bone = root->bones[j];
        if (bone->x18 >= 0) {
            SUBN(&delta, p, q);
            STORE(x18, VLength(&delta));
        }
        bone = root->bones[curr];
        func_00221AB8(&result, active, previous);
        previous = active;
        func_00221D58(&temp, &bone->x40);
        func_00221930(&result, &temp, &result);
        func_00221D58(&temp, &bone->x80);
        func_00221930(&result, &result, &temp);
        func_002224E0(&result, &rot);
        STORE(x12, rot.x);
        STORE(x14, rot.y);
        STORE(x16, rot.z);
        active = next;
        next = previous;
        i = next_i;
    }
    func_00221AB8(&result, &D_00769830[root->xE4], previous);
    bone = root->bones[2];
    if (root->x100) {
        func_00221D58(&temp, &bone->x40);
        func_00221930(&result, &temp, &result);
        func_00221D58(&temp, &bone->x80);
        func_00221930(&result, &result, &temp);
        func_002224E0(&result, &rot);
        STORE(x12, rot.x);
        STORE(x14, rot.y);
        STORE(x16, rot.z);
    } else {
        STORE(x12, 0.0f);
        STORE(x14, 0.0f);
        STORE(x16, 0.0f);
    }
}
