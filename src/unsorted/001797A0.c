/* UNIT 0x001797A0 0x00179A38 */
#include "rider_geometry.h"
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct RiderMarkerHud {
    char p0[0xc80];
    long *xC80;
    char p1[0x1164 - 0xc84];
    void *x1164;
    char p2[0x1280 - 0x1168];
    void *x1280;
} Obj;
typedef struct Rider {
    char p0[0x792c];
    char *x792C;
    char p1[0x7978 - 0x7930];
    int x7978;
    char p2[0x798c - 0x797c];
    int x798C;
    char p3[0x7a5c - 0x7990];
    int x7A5C;
    char p4[0x7bd0 - 0x7a60];
} Rider;
typedef struct World {
    Rider riders[10];
    char pad[0x501a8 - 10 * 0x7bd0];
    int count;
} World;
extern World D_002AF2C0;
extern void func_0023D618(void *, int *, int *);
extern void func_0023D650(void *, int *, int *);
extern void func_00222D48(void *, Vec *, Vec *);

typedef unsigned int u128 __attribute__((mode(TI)));
#define COPY_VECTOR(dst, src) (*(u128 *)&(dst) = *(u128 *)((src) + 16))

void func_001797A0(Obj *obj) {
    Vec pos, screen;
    World *world;
    int cx, cy, rx, ry;
    int i = 0, count;
    int xmax, xmin, ymax, ymin;
    Rider *r;
    float z, alpha;
    float nearDepth;
    int x, y;
    long *packet;

    func_0023D618(obj->x1280, &cx, &cy);
    func_0023D650(obj->x1280, &rx, &ry);
    xmin = cx - rx;
    xmax = cx + rx;
    ymin = cy - ry;
    ymax = cy + ry;
    obj->xC80[0] = ((unsigned long)0x8000 << 45) | 0x8001;
    obj->xC80[1] = 14;
    packet = obj->xC80;
    obj->xC80 = packet + 2;
    packet[2] = 0x44;
    obj->xC80[1] = 0x42;
    obj->xC80 += 2;
    count = D_002AF2C0.count;
    for (; i < count; ++i) {
        world = &D_002AF2C0;
        nearDepth = 150.0f;
        r = (Rider *)((char *)world + i * 0x7bd0);
        {
            int active = r->x7A5C;
            void *current = obj->x1164;
            if (active == 1 && r != current) {
                COPY_VECTOR(pos, r->x792C);
                pos.z += 5.0f;
                func_00222D48(obj->x1280, &pos, &screen);
                alpha = 170.0f;
                z = screen.z;
                if (z > nearDepth && z < 200.0f) {
                    z = 1.0f - (200.0f - z) * 0.02f;
                    alpha = z * 170.0f;
                    z = screen.z;
                }
                if (z > nearDepth) {
                    float depth = z;
                    z = screen.x;
                    if (z > 0.0f) z += 0.5f;
                    else z -= 0.5f;
                    x = (int)z;
                    z = screen.y;
                    y = (int)(z > 0.0f ? (z += 0.5f) : (z -= 0.5f));
                    if (depth > 1.0f && xmin < x && x < xmax && ymin < y && y < ymax)
                        func_0017B848((RiderMarkerHud *)(obj), x, y, r->x798C, r->x7978, alpha);
                }
            }
        }
    }

}
