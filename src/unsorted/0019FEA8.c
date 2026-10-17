/* UNIT 0x0019FEA8 0x0019FF58 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct { char pad[16]; u128 x10; } Data;
typedef struct Node { unsigned int flags; Data *x4; } Node;
typedef struct { char pad[0x18]; int x18; char pad1[0x484-0x1C]; int x484; } Obj;
void func_0019FEA8(Obj *obj, Node *node) {
    if ((node->flags & 0x3F) == 11) {
        *(u128 *)(((node->flags >> 3) & 0x7FF0) + (unsigned int)obj + 0x230) = node->x4->x10;
        if ((node->flags & 0xFFFC0000) == 0x7DCC0000) {
            *((char *)obj + ((node->flags >> 7) & 0x7FF) + 0x460) = 1;
            obj->x18 = 1;
        } else {
            *((char *)obj + ((node->flags >> 7) & 0x7FF) + 0x460) = 0;
        }
        obj->x484++;
        *(Node **)((char *)obj + ((node->flags >> 5) & 0x1FFC) + 0x1A0) = node;
        node->flags &= 0xFFFC007F;
    }
}
