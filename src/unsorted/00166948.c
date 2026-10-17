/* UNIT 0x00166948 0x001669D8 */
typedef struct Obj { char pad0[0x100]; unsigned long *x100; char pad104[0x98]; unsigned char x19C; char pad19D[0x2B7]; unsigned char x454; } Obj;
extern char D_0038DCE0[];
extern void func_00166B30(Obj *, float);
extern void func_00164A50(void *, Obj *);
void func_00166948(Obj *obj) {
 *obj->x100=(*obj->x100 & 0xFDFFFFFFFFFFFFFFUL) | ((unsigned long)(obj->x454 & 1)<<57);
 obj->x100=0;
 if(obj->x19C) func_00166B30(obj,100.0f);
 func_00164A50(D_0038DCE0,obj);
}
