/* UNIT 0x001AC350 0x001AC3F0 .rodata=0x28E8E0:0x28E900 */
typedef struct Node { unsigned int flags; char pad[12]; float x10, x14, x18; } Node;
typedef struct { char pad[0x48A0]; Node *x48A0; char pad2[0x6208-0x48A4]; Node *x6208; } Inner;
typedef struct { char pad[8]; Node *x8, *xC, *x10; char pad2[0x16D0-0x14]; Inner *x16D0; } Obj;
extern void func_00157890(void *, Node *);
extern void func_001AE998(void *, float, float, float);
void func_001AC350(Obj *obj, Node *node) {
    switch (node->flags >> 18) {
    case 0x3E9: obj->x16D0->x48A0 = node; break;
    case 0x3EA: func_001AE998((char *)obj->x16D0 + 0x46D0, node->x10, node->x14, node->x18); break;
    case 0x3EB: obj->x8 = node; break;
    case 0x3EC: obj->x16D0->x6208 = node;
    case 0x3EE: func_00157890((char *)obj->x16D0 + 0x5D00, node); break;
    case 0x3EF: obj->xC = node;
    case 0x3F0: obj->x10 = node;
    default: break;
    }
}
