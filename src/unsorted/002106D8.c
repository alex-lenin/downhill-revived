/* UNIT 0x002106D8 0x00210738 */
struct Obj { char pad[0x34]; unsigned int *x34; };
struct Ref { int x0,x4,x8,xC; };
extern void func_00203590(struct Ref *,unsigned int,int,int);
extern void func_00207D90(struct Obj *,struct Ref *);
extern void func_00203B68(struct Ref *,int,float);
void func_002106D8(struct Obj *obj) {
    struct Ref ref;
    if (obj->x34) {
        ref.x8=0;
        func_00203590(&ref,*obj->x34>>18,0,0);
        func_00207D90(obj,&ref);
        func_00203B68(&ref,1,0.0f);
    }
}
