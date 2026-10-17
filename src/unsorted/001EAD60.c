/* UNIT 0x001EAD60 0x001EAE58 */
struct Elem { char pad0[0x50]; };
struct Target { char pad0[0x10]; char x10[0x10]; };

struct Obj {
    struct Elem elems[3];
    unsigned char active;
    unsigned char started;
    unsigned char xF2;
    char padF3[0xD];
    char x100[0x50];
    int count;
};

extern int func_001EAE58(struct Obj *obj);
extern void func_001EAF58(struct Elem *elem);
extern void func_001EAFD8(struct Elem *elem);
extern void func_001D3080(void *p);
extern void func_001D30A8(void *p, void *target);

void func_001EAD60(struct Obj *obj)
{
    int i;
    int j;

    if (!obj->active) {
        return;
    }
    if (!obj->started) {
        if (!obj->xF2 && func_001EAE58(obj)) {
            obj->started = 1;
            for (i = 0; i < obj->count; i++) {
                func_001EAF58(&obj->elems[i]);
            }
        }
        if (!obj->started) {
            func_001D3080(obj->x100);
            return;
        }
    }
    for (j = 0; j < obj->count; j++) {
        func_001EAFD8(&obj->elems[j]);
    }
    if (obj->started) {
        func_001D30A8(obj->x100, ((struct Target **)obj)[0x10]->x10);
    }
}
