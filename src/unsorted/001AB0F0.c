/* UNIT 0x001AB0F0 0x001AB418 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct Body {
    char pad0[0x38];
    void *x38;
} Body;

typedef struct State {
    char pad0[4];
    float x4;
    float x8;
    float xC;
    char pad10[0x10];
    int numPoints;
    char pad24[8];
    Vec3 *points;
    char pad30[0x10];
    Body body;
} State;

typedef struct Obj {
    char pad0[0x20];
    State *state;
} Obj;

typedef struct Input {
    char pad0[0x10];
    Vec pos;
    Vec dir;
} Input;

/* The (up to) 4 highest-scoring points, best first. */
typedef struct Top4 {
    int count;
    short indices[4];
    float scores[4];
} Top4;

extern void func_00221888(void *, Vec *, Vec *);
extern void func_001AB768(Obj *, Vec *, Vec3 *);
extern void func_00232A80(Body *, Vec *, Vec *);
extern void func_002320B0(Body *, Vec *, Vec *, float, float, float);

#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

static inline float Dot3(Vec3 *a, Vec *b) {
    float r;
    __asm__ volatile(
        "mula.s %1, %2\n\t"
        "madda.s %3, %4\n\t"
        "madd.s %0, %5, %6"
        : "=f"(r)
        : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

static inline float Dot(Vec *a, Vec *b) {
    float r;
    __asm__ volatile(
        "mula.s %1, %2
	"
        "madda.s %3, %4
	"
        "madd.s %0, %5, %6"
        : "=f"(r)
        : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    return r;
}

static inline void Top4_Insert(Top4 *t, int index, float score) {
    int j;
    int k;
    for (j = 0; j < t->count && !(score <= t->scores[j]); j++) {
    }
    if (j != 4) {
        if (t->count < 4) {
            t->count++;
        }
        for (k = t->count - 1; j < k; k--) {
            t->indices[k] = t->indices[k - 1];
            t->scores[k] = t->scores[k - 1];
        }
        t->scores[j] = score;
        t->indices[j] = index;
    }
}

static inline void Sub(Vec *out, Vec *a, Vec *b) {
    __asm__ volatile(
        "lqc2 $vf11, 0x0(%1)\n\t"
        "lqc2 $vf12, 0x0(%2)\n\t"
        "vsub.xyz $vf11, $vf11, $vf12\n\t"
        "sqc2 $vf11, %0"
        : "=m"(*out)
        : "r"(a), "r"(b)
        : "memory");
}

int func_001AB0F0(Obj *obj, Input *input) {
    Vec contacts[4];
    Vec local;
    Top4 top;
    Vec delta;
    Vec *dir;
    int count;
    Body *body;
    State *state;
    int numPoints;
    int changed;
    float dot;
    float absDot;

    state = obj->state;
    numPoints = state->numPoints;
    body = &state->body;

    if (numPoints == 0) {
        COPY_VECTOR(contacts[0], &input->pos);
        count = 1;
    } else {
        int i;
        int n;

        dir = &input->dir;
        func_00221888(body->x38, dir, &local);
        top.count = 0;
        for (i = 0; i < numPoints; i++) {
            char *points;
            int offset;
            points = (char *)obj->state->points;
            offset = i * sizeof(Vec3);
            Top4_Insert(&top, i, Dot3((Vec3 *)(points + offset), &local));
        }

        __asm__ volatile ("Lab0f0_scoredA:");
        __asm__ volatile ("Lab0f0_scoredB:");
        __asm__ volatile ("Lab0f0_scoredC:");

        n = 0;
        count = 0;
        if (top.count > 0) {
            Vec *out;
            short *index;
            int stride;
            stride = sizeof(Vec3);
            index = top.indices;
            out = contacts;
            for (;;) {
                func_001AB768(obj, out, (Vec3 *)((char *)obj->state->points + *index * stride));
                Sub(&delta, out, contacts);
                dot = Dot(&delta, &input->dir);
                absDot = __builtin_fabsf(dot);
                __asm__ volatile ("Lab0f0_dot_live:" : : "f"(absDot), "f"(dot));
                if (absDot < 0.05f) {
                    out++;
                    count++;
                }
                n++;
                index++;
                if (n >= top.count) break;
            }
        }
    }

    changed = 0;
    for (numPoints = count - 1; numPoints >= 0; numPoints--) {
        Vec *contact;
        int different;
        contact = &contacts[numPoints];
        func_00232A80(body, &local, contact);
        func_002320B0(body, contact, &input->dir,
                      obj->state->x4, obj->state->x8, obj->state->xC);
        /* the result reuses top's stack slot */
        func_00232A80(body, (Vec *)&top, contact);
        different = 0;
        if (local.x != ((Vec *)&top)->x || local.y != ((Vec *)&top)->y ||
            local.z != ((Vec *)&top)->z) {
            different = 1;
        }
        changed |= different;
    }

    return changed;
}
