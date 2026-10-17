/* UNIT 0x00165648 0x001656A0 */
struct Vec { float x, y, z; };
struct Other { char pad[0x50]; struct Vec x50; };
struct Obj { char pad[0x100]; struct Other *x100; char pad104[0x2C]; struct Vec x130; };
int func_00165648(struct Obj *obj) {
    struct Vec *a = &obj->x100->x50;
    struct Vec *b = &obj->x130;
    float result;
    int positive = 0;
    __asm__ volatile ("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(result) : "f"(a->x), "f"(b->x), "f"(a->y), "f"(b->y), "f"(a->z), "f"(b->z));
    if (result > 0.0f) positive = 1;
    return positive;
}
