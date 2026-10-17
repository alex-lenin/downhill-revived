/* UNIT 0x00157AB8 0x00157B88 */
typedef struct Owner {
    char pad0[0x4098];
    unsigned long x4098;
} Owner;

typedef struct Node {
    union {
        unsigned long flags;
        struct {
            char pad0[4];
            unsigned char x4;
        } b;
    } u;
    char pad8[0x30 - 0x8];
    float scale[3];
} Node;

typedef struct Obj {
    Owner *owner;
    char pad4[0x110 - 0x4];
    int count;
    int items[2];
    Node *node;
} Obj;

extern void func_00157B88(Obj *obj);
extern void func_00157BE8(Obj *obj);
extern void func_00218498(int item, int flag);

unsigned char func_00157AB8(Obj *obj)
{
    int i;
    unsigned char flag;
    float *s;

    i = 0;
    func_00157B88(obj);
    func_00157BE8(obj);
    flag = (unsigned char)(obj->owner->x4098 >> 2) & 1;
    obj->node->u.b.x4 = flag;
    obj->node->u.flags |= 0x0100000000000000;
    s = obj->node->scale;
    s[0] = 1.5f;
    s[1] = 1.5f;
    s[2] = 1.5f;
    for (; i < obj->count; i++) {
        func_00218498(obj->items[i], flag);
    }
    return flag;
}
