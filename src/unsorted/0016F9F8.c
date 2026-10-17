/* UNIT 0x0016F9F8 0x0016FA88 .rodata=0x28B080:0x28B0C0 */
typedef struct { char pad0[0x88]; int x88; char pad8C[0x154]; char x1E0[0x20]; int *x200; } Obj;
extern void func_0016FA88(Obj *, int, void *, int);
void func_0016F9F8(Obj *obj) {
    int value;
    if (obj->x200) {
        value = obj->x88;
        switch (*obj->x200) {
        case 1: case 2: case 3: case 4: case 7: func_0016FA88(obj, value, obj->x1E0, 1); break;
        case 5: case 6: case 8: case 9: func_0016FA88(obj, value, obj->x1E0, 0); break;
        case 10: case 11: func_0016FA88(obj, value, obj->x1E0, -1); break;
        case 12: case 13: func_0016FA88(obj, value, obj->x1E0, 2); break;
        case 0: case 14: default: break;
        }
    }
}
