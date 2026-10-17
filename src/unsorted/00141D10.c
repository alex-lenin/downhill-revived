/* UNIT 0x00141D10 0x00141D60 */
struct Obj { char pad0[0x68]; char x68[0x10]; char x78[0x78]; int xF0; int xF4; int xF8; };
extern void func_00203590(void *, int, int, int);
void func_00141D10(struct Obj *obj) {
    if (obj->xF0) {
        func_00203590(obj->x68, obj->xF4, obj->xF8, 0);
        func_00203590(obj->x78, obj->xF4, obj->xF8, 1);
    }
}
