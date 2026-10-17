/* UNIT 0x001EA918 0x001EA9A0 */
#include "rider.h"
struct Game {
    char pad0[0x6D28];
    int x6D28;
    char pad6D2C[0x792C - 0x6D2C];
    SceneNode *bodyNode;
};

struct Obj {
    struct Game *x0;
    char pad4[0x14 - 0x4];
    int x14;
    int x18;
    int x1C;
    int x20;
    char pad24[0x1D0 - 0x24];
    float x1D0;
    float x1D4;
};



void func_001EA918(struct Obj *obj)
{
    float angles[4];
    struct Game *game = obj->x0;

    func_002224E0(&game->bodyNode->matrix, (Vector3 *)angles);
    obj->x20 = 0;
    obj->x14 = 0;
    obj->x18 = 0;
    obj->x1C = 0;
    obj->x1D0 = angles[2] * 57.29578f + 180.0f;
    obj->x1D4 = angles[0] * 57.29578f + 180.0f;
    game->x6D28 = 0;
}
