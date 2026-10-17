/* UNIT 0x00177D28 0x00177DA0 */
struct Quad {
    int x0;
    int x4;
    int x8;
    int xC;
};

struct Obj {
    char pad0[0xC84];
    struct Quad quads[38];
    char padEE4[0xC];
    struct Quad *dst[4];
    char padF00[0x134A - 0xF00];
    unsigned char cur[5];
    unsigned char next[4];
};

void func_00177D28(struct Obj *obj)
{
    int i;
    struct Quad *src;

    for (i = 0; i < 4; i++) {
        if (obj->cur[i] != obj->next[i]) {
            src = &obj->quads[obj->next[i]];
            obj->dst[i]->x0 = src->x0;
            obj->dst[i]->x4 = src->x4;
            obj->dst[i]->x8 = src->x8;
            obj->dst[i]->xC = src->xC;
        }
    }
}
