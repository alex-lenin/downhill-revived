/* UNIT 0x00177F08 0x00177F80 */
struct Quad {
    int x0;
    int x4;
    int x8;
    int xC;
};

struct Obj {
    char pad0[0xC84];
    struct Quad quads[38];
    struct Quad *dst[2];
    char padEEC[0x1344 - 0xEEC];
    unsigned char cur[3];
    unsigned char next[2];
};

void func_00177F08(struct Obj *obj)
{
    int i;
    struct Quad *src;

    for (i = 0; i < 2; i++) {
        if (obj->cur[i] != obj->next[i]) {
            src = &obj->quads[obj->next[i]];
            obj->dst[i]->x0 = src->x0;
            obj->dst[i]->x4 = src->x4;
            obj->dst[i]->x8 = src->x8;
            obj->dst[i]->xC = src->xC;
        }
    }
}
