/* UNIT 0x00210760 0x00210860 */
typedef struct Header {
    unsigned int type : 6;
    unsigned int pad : 12;
    unsigned int id : 14;
} Header;

typedef struct Shape {
    Header hdr;
    char pad4[0xC];
    float center[3];
    float radius;
} Shape;

typedef struct Node {
    Header *x0;
    unsigned char x4;
    char pad5[3];
    float x8;
} Node;

typedef struct Obj {
    char pad0[0x30];
    Node *x30;
    Shape *x34;
} Obj;

extern Node *func_00208FB8(void);
extern void func_00218278(Node *, Shape *);

void func_00210760(Obj *obj)
{
    Shape *shape;

    obj->x30 = func_00208FB8();
    func_00218278(obj->x30, obj->x34);
    shape = obj->x34;
    if (shape->hdr.type == 1) {
        float v[3];
        float length;
        float x = shape->center[0], y = shape->center[1], z = shape->center[2];
        v[0] = x;
        v[1] = y;
        v[2] = z;
        __asm__ ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
                 : "=&f"(length) : "f"(v[0]), "f"(v[1]), "f"(v[2]) : "memory");
        obj->x30->x8 = shape->radius + length;
    } else {
        obj->x30->x8 = 30.0f;
    }
    obj->x30->x0->id = obj->x34->hdr.id;
    *(long *)obj->x30 |= 1L << 57;
    *(long *)obj->x30 |= 1L << 58;
    obj->x30->x4 = 1;
}
