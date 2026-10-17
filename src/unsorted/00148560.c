/* UNIT 0x00148560 0x001486A8 */
struct Sub {
    char pad0[0x3514];
    char *x3514;
};

struct Obj {
    char pad0[0x90];
    float x90[0x89];
    struct Sub *x2B4;
    int x2B8;
};

extern void func_00148700(struct Obj *obj);
extern void func_0014BE48(struct Obj *obj);
extern void func_00148740(struct Obj *obj);
extern void func_0014BEB8(struct Obj *obj, int mode);
extern void func_00222460(struct Obj *obj, float *out);
extern void func_001486A8(struct Obj *obj);
extern int func_00238F30(void);
extern void func_00209248(void *a, void *b, void *c, float scale);

void func_00148560(struct Obj *obj, int state)
{
    char *p;

    if (state == 7 && obj->x2B8 != 7) {
        func_00148700(obj);
    } else if (obj->x2B8 == 7 && state != 7) {
        func_0014BE48(obj);
    } else if (state == 2 && obj->x2B8 != 2) {
        p = obj->x2B4->x3514;
        func_00209248(p, p + 0x10, p + 0x40, 1.0f / (float)func_00238F30());
    } else if (state == 4) {
        func_00148740(obj);
    }
    obj->x2B8 = state;

    if (state == 5) {
        func_0014BEB8(obj, 0);
        func_00222460(obj, obj->x90);
    } else if (state == 6) {
        func_0014BEB8(obj, 1);
        func_00222460(obj, obj->x90);
    } else if (state == 9) {
        func_0014BEB8(obj, 1);
        func_00222460(obj, obj->x90);
    }
    func_001486A8(obj);
}
