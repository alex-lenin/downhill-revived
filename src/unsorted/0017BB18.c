/* UNIT 0x0017BB18 0x0017BB90 */
struct Vec { int x0, x4, x8, xC; };
struct Obj { char pad0[0xC84]; struct Vec xC84; char padC94[0xF04-0xC94]; struct Vec *xF04[10]; long *xF2C[15]; int xF68; };
void func_0017BB18(struct Obj *obj) {
    struct Vec *src = &obj->xC84;
    struct Vec **dst;
    long **flags;
    int count;
    long mask;
    if (obj->xF68) {
        mask = ~0x3FL;
        flags = obj->xF2C;
        dst = obj->xF04;
        count = 5;
        do {
            count--;
            (*dst)->x0 = obj->xC84.x0;
            (*dst)->x4 = src->x4;
            (*dst)->x8 = src->x8;
            (*dst)->xC = src->xC;
            dst++;
            **flags &= mask;
            flags++;
        } while (count >= 0);
    }
}
