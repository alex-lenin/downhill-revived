/* UNIT 0x00171128 0x001711B8 */
struct Vec { float x,y,z,w; };
struct Obj { char *x0; int x4,x8,index; };
extern void func_001AC578(void *);
extern unsigned char func_00204230(struct Vec *);
extern void func_0012FE38(void *,void *);
void func_00171128(struct Obj *obj) {
    struct Vec value;
    char *base = obj->x0;
    char *part = base + 0x1E40;
    func_001AC578(part);
    part += obj->index * 16 + 0x14;
    value = *(struct Vec *)part;
    if ((func_00204230(&value) ^ 1) & 0xFF) func_0012FE38(base,base + 0x4ED0);
}
