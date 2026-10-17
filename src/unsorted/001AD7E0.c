/* UNIT 0x001AD7E0 0x001AD8B0 */
struct Anim {
    char pad0[0x9];
    unsigned char x9;
    char padA[0x5A];
};

struct Obj {
    char pad0[0x30];
    struct Anim x30;
    struct Anim x94;
    char *xF8;
};

extern void func_001AD8B0(struct Obj *);
extern void func_001AC7B0(struct Anim *, void *);
extern void func_001AC810(struct Anim *, float);
extern void func_001ACAE0(struct Anim *, float);
extern void func_001ACB70(struct Anim *, float);

void func_001AD7E0(struct Obj *obj, char *data)
{
    struct Anim *anim;

    if (data != 0) {
        obj->xF8 = data;
        func_001AD8B0(obj);
        func_001AC7B0(&obj->x30, obj->xF8 + 0x60);
        func_001AC810(&obj->x30, 0.0f);
        func_001ACAE0(&obj->x30, 0.5f);
        func_001ACB70(&obj->x30, 0.0f);
        anim = &obj->x94;
        func_001AC7B0(anim, obj->xF8 + 0x1ED4);
        func_001AC810(anim, 0.0f);
        func_001ACAE0(anim, 0.5f);
        anim->x9 = 4;
        func_001ACB70(anim, 0.0f);
    }
}
