/* UNIT 0x00162DE8 0x00162F38 */
struct Vec6 {
    float x0;
    float x4;
    float x8;
    float xC;
    float x10;
    float x14;
};

struct Msg {
    int type;
    int x4;
    struct Vec6 v;
};

extern struct Vec6 D_00768498;

int func_00162DE8(struct Msg *msg)
{
    int size = 8;
    struct Vec6 *v;

    switch (msg->type) {
    case 1:
        D_00768498.x0 = msg->v.x0;
        D_00768498.x4 = msg->v.x4;
        D_00768498.x8 = 90.0f;
        D_00768498.xC = 1.0f;
        D_00768498.x10 = 1.0f;
        D_00768498.x14 = 1.0f;
        size = 0x10;
        break;
    case 2:
        v = &msg->v;
        D_00768498.x0 = v->x0;
        D_00768498.x4 = v->x4;
        D_00768498.x8 = v->x8;
        D_00768498.xC = 1.0f;
        D_00768498.x10 = 1.0f;
        D_00768498.x14 = 1.0f;
        size = 0x14;
        break;
    case 3:
        v = &msg->v;
        D_00768498.x0 = v->x0;
        D_00768498.x4 = v->x4;
        D_00768498.x8 = v->x8;
        D_00768498.xC = v->xC;
        D_00768498.x10 = v->x10;
        D_00768498.x14 = 1.0f;
        size = 0x1C;
        break;
    case 4:
        D_00768498 = msg->v;
        size = 0x20;
        break;
    }
    return size;
}
