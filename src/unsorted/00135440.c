/* UNIT 0x00135440 0x00135478 */
struct Rider {
    char pad0[0x796C];
    int x796C;
};

struct Obj {
    char pad0[0x158];
    struct Rider *x158;
};

float func_00135440(struct Obj *obj) {
    float ret;

    if (obj->x158) {
        switch (obj->x158->x796C) {
        case 0xC0:
            ret = 1.0f;
            break;
        case 0x120:
            ret = 1.0f;
            break;
        case 0x20:
        default:
            ret = 1.0f;
            break;
        }
    }
    ret = 1.0f;
    return ret;
}
