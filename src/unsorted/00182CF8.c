/* UNIT 0x00182CF8 0x00182D78 */
typedef struct Child { int x0; char x4; char pad[11]; int x10; float x14,x18; } Child;
typedef struct Obj { int x0,x4; Child *x8; } Obj;
extern Child *func_0023D040(int);
extern Child *func_00208FB8(void);
extern void func_00218278(Child *,int);
void func_00182CF8(Obj *obj,int arg) {
 Child *q; Child *p=func_0023D040(arg);
 p->x14=-128.07f; p->x18=-8.76f; p->x10=0;
 q=func_00208FB8(); obj->x8=q; q->x4=1;
 func_00218278(obj->x8,obj->x0);
 obj->x8->x10=0; obj->x8->x14=0; obj->x8->x18=0;
}
