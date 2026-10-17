/* UNIT 0x00158158 0x00158320 */
typedef struct Vec { float x,y,z,w; } Vec __attribute__((aligned(16)));
typedef struct Obj { char pad0[0x10]; Vec x10; char pad20[0x20]; Vec x40; Vec x50; } Obj;
typedef struct World { Obj *objects[100]; float radius[100]; int count; } World;
extern float D_0029C6C0[];
extern void func_00221888(Vec *,Vec *,Vec *);
static inline void Sub(Vec *out,Vec *a,Vec *b) { __asm__ volatile("lqc2 $vf11, 0(%1)\n\tlqc2 $vf12, 0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*out) : "r"(a),"r"(b)); }
static inline float Dot(Vec *a,Vec *b) { float r; __asm__ volatile("mula.s %1, %2\n\tmadda.s %3, %4\n\tmadd.s %0, %5, %6" : "=f"(r) : "f"(a->x),"f"(b->x),"f"(a->y),"f"(b->y),"f"(a->z),"f"(b->z)); return r; }
int func_00158158(World *world,Obj *obj,float *out,float low,float high) {
    Vec delta, normal, result;
    Obj *found;
    float best, lower, upper;
    int i;
    best = D_0029C6C0[0];
    lower = low + obj->x10.z;
    upper = high + obj->x10.z;
    found = 0;
    for (i = 0; i < world->count; i++) {
        Obj *other = world->objects[i];
        float len, inv, dist, proj, r;
        int bits;

        if (lower < other->x10.z && other->x10.z < upper) {
            Sub(&delta, &other->x10, &obj->x10);
            if (Dot(&obj->x50, &delta) > 0.0f) {
                /* Sony length/reciprocal sequence (sqrt.s, then 1.0f/len) */
                __asm__ volatile("mula.s %3,%3\n\tmadda.s %4,%4\n\tmadd.s %1,%5,%5\n\tsqrt.s %1,%1\n\tlui %2,0x3f80\n\tmtc1 %2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(len),"=r"(bits) : "f"(delta.x),"f"(delta.y),"f"(delta.z));
                normal.x = delta.x * inv;
                normal.y = delta.y * inv;
                normal.z = delta.z * inv;
                dist = len * len;
                if (dist < best) {
                    proj = len * Dot(&obj->x50, &normal);
                    r = world->radius[i];
                    if (dist - proj * proj < r * r) {
                        found = other;
                        best = dist;
                        func_00221888(&obj->x40, &delta, &result);
                        *out = result.x;
                    }
                }
            }
        }
    }
    return found != 0;
}
