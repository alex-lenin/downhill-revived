/* UNIT 0x0011FAA0 0x0011FB70 */
typedef struct Value { int x[4]; } Value;
typedef struct Obj { char pad[0x3c]; Value x3c; } Obj;
extern float func_00203C28(Value *);
extern unsigned char func_00204230(Value *);
extern void func_00202E38(Value *,int,int,float);
extern void func_0011FC70(Obj *);
void func_0011FAA0(Obj *obj) {
    Value a;
    Value b;
    int flag=0;
    a=obj->x3c;
    if(func_00203C28(&a)>=1.0f || (b=obj->x3c, !func_00204230(&b))) flag=1;
    if((unsigned char)flag) func_00202E38(&obj->x3c,1,1,600.0f);
    func_0011FC70(obj);
}
