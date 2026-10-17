/* UNIT 0x001AA318 0x001AA688 */
#include "scene.h"
#include "timer.h"
typedef struct Header {
    int x0;
    int x4;
} Header;

typedef struct Sphere {
    float x;
    float y;
    float z;
    float radius;
} Sphere;

typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;


typedef SceneNode Node;

typedef struct Body {
    float inverse_mass;
    char pad4[0x2C];
    void *x30;
    void *x34;
    void *x38;
    void *x3C;
    void *x40;
} Body;

typedef struct Model {
    float mass;
    char pad4[0x18];
    int count;
    char pad20[4];
    int x24;
    Sphere *spheres;
    char pad2C[4];
    float x30;
    float x34;
    float x38;
    char pad3C[4];
    Body body;
} Model;

struct A {
    int x0;
    float x4;
    float x8;
    float xC;
    short x10;
    short x12;
    short x14;
    short x16;
    Header *x18;
    int *x1C;
    Model *x20;
    Node *x24;

    int func_001AA318();
};

extern Model *func_001AAC58();
extern Node *func_00208FB8();
extern void func_001AACA8(Model *);
extern int func_001AA7D8(Header *, Model *);
extern void func_001AA688(A *);
extern void func_002230B0(float *, float *);
extern void func_002232C8(float (*)[4], float *);
extern void func_002319A0(Body *, float, float, float);
extern void func_00231AF8(Body *, float, float, float, float);
extern void func_00231BD8(Body *);
extern float D_0029D3C8[];
extern float D_0029D3CC[];
extern char D_00759780[];
extern void func_002624C0(void);

static inline float minf(float a, float b)
{
    float r;
    __asm__ volatile ("min.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

static inline float maxf(float a, float b)
{
    float r;
    __asm__ volatile ("max.s %0,%1,%2" : "=&f"(r) : "f"(a), "f"(b));
    return r;
}

int A::func_001AA318()
{
    Node *node;
    Body *body;
    Model *setup_model;
    Model *physics_model;
    int i;

    x20 = func_001AAC58();
    if (x20 == 0) {
        return 0;
    }

    node = x24;
    if (node == 0) {
        node = func_00208FB8();
        if (node == 0) {
            func_001AACA8(x20);
            return 0;
        }
    }

    if (x24 == 0) {
        float q[4];
        Vec4 *pos;
        float px;
        float py;
        float pz;

        q[0] = (float)x10 * 0.00003051851f;
        q[1] = (float)x12 * 0.00003051851f;
        q[2] = (float)x14 * 0.00003051851f;
        q[3] = (float)x16 * 0.00003051851f;
        func_002230B0(q, q);
        func_002232C8((float (*)[4])(&node->matrix), q);

        px = x4;
        py = x8;
        pz = xC;
        pos = (Vec4 *)&node->position;
        pos->x = px;
        pos->y = py;
        pos->z = pz;
        pos->w = 0.0f;
        node->resourceId = x18->x4;

        if (x1C != 0) {
            *x1C = (*x1C & -0x40) | 9;
        }

        node->x4 = 1;
        node->flags |= (SceneNodeFlags)1 << 57;
        if (x1C != 0) {
            node->xB4 = ((unsigned int)*x1C >> 7) & 0x7FF;
        }
        node->flags |= (SceneNodeFlags)1 << 58;
        x24 = node;
    }

    if (!func_001AA7D8(x18, x20)) {
        func_001AA688(this);
        return 0;
    }

    setup_model = x20;
    setup_model->x24 = 0;
    physics_model = x20;
    body = &physics_model->body;
    body->x34 = &node->position;
    body->x38 = (&node->matrix);
    if (node->flags & ((SceneNodeFlags)1 << 58)) {
        body->x3C = (&node->x90);
        body->x40 = (&node->xC0);
        body->x30 = &node->xB0;
    }

    physics_model->body.inverse_mass = 1.0f / x20->mass;
    func_002319A0(body, x20->x30, x20->x34, x20->x38);

    Model *geom;
    Vec4 mins;
    Vec4 maxs;

    mins.x = D_0029D3C8[0];
    mins.y = D_0029D3C8[0];
    mins.z = D_0029D3C8[0];
    Vec4 *maxp = &maxs;
    maxp->x = D_0029D3CC[0];
    geom = x20;
    maxp->y = D_0029D3CC[0];
    maxp->z = D_0029D3CC[0];

    /* (int)geom - (int)x20 is always 0 (both hold this->x20); it keeps geom's
       load live so the loop bound/base allocate exactly as in retail. */
    for (i = (int)geom - (int)x20; i < x20->count; i++) {
        Sphere &sphere = x20->spheres[i];
        float radius;

        radius = sphere.radius;
        mins.x = minf(mins.x, sphere.x - radius);
        mins.y = minf(mins.y, sphere.y - radius);
        mins.z = minf(mins.z, sphere.z - radius);
        maxs.x = maxf(maxs.x, sphere.x + radius);
        maxs.y = maxf(maxs.y, sphere.y + radius);
        maxs.z = maxf(maxs.z, sphere.z + radius);
    }

    func_00231AF8(body,
                  maxs.x - mins.x,
                  maxs.y - mins.y,
                  maxs.z - mins.z,
                  x20->mass);
    func_00231BD8(body);
    func_00236B28((TimerPool *)(D_00759780), (TimerCallback)(func_002624C0), this, 1);
    return 1;
}
