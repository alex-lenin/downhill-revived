/* UNIT 0x0018E680 0x0018E6B8 */
struct Rect {
    short x0;
    short x2;
    char pad4[3];
    char x7;
};

struct Obj {
    char pad0[0x438];
    struct Rect x438;
    char pad440[0x460 - 0x440];
    int x460;
    int x464;
    int x468;
};

void func_0018E680(struct Obj *obj)
{
    struct Rect *r = &obj->x438;
    int w = obj->x468;
    int y = obj->x460 - 4;
    int x = obj->x464;
    r->x0 = x;
    r->x7 = 0;
    r->x2 = y - (w + w / 2);
}
