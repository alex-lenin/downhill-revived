/* UNIT 0x001B0C10 0x001B0D38 */
#include "camera.h"
#include "rider.h"
typedef struct Vec3 { float x, y, z; } Vec3;

typedef Rider World;

typedef RiderComponent Level;

typedef RiderCamera Cam;

extern void func_001B0F80(void *p, Level *level);
extern void func_001AE9B0(void *p, Level *level);
extern void func_0023CB00(int);
extern void func_002185C0(void *p, int);

extern void func_001AEA30(void *p, Level *level);
extern void func_001B1068(void *p);


static inline void Vec3_Zero(Vector4 *v)
{
    v->x = 0.0f;
    v->y = 0.0f;
    v->z = 0.0f;
}

void func_001B0C10(Cam *cam, Level *level)
{
    World *world;

    cam->xA10 = level;
    if (level->node == 0)
        return;
    if (!cam->x5B0) {
        world = level->rider;
        func_001B0F80(cam->x5C0, level);
        func_001AE9B0(world->x46D0, level);
        func_0023CB00(0);
        func_002185C0(cam->xA10->node, 0);
        func_002185C0(cam->xA10->rider->bodyNode, 0);
        func_001B30B8(cam, cam->xA10);
        func_001AEA30(world->x46D0, level);
        cam->x5B0 = 1;
        func_001B1068(cam->x5C0);
        return;
    }
    func_0023CB00(0);
    func_002185C0(cam->xA10->node, 0);
    func_002185C0(cam->xA10->rider->bodyNode, 0);
    Vec3_Zero(&cam->xA50);
    Vec3_Zero(&cam->xA60);
    func_001B3108((RiderCamera *)(cam));
}
