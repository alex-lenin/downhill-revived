/* UNIT 0x00208500 0x00208660 */
typedef struct Handle { int x0,x4,x8,xC; } __attribute__((packed)) Handle;
typedef struct Obj { Handle *x0; int x4,x8,xC; char pad[0xC0-0x10]; int xC0; } Obj;
extern void func_00204080(Handle *, int);
extern void func_00202E38(Handle *, int, int, float);
extern float func_00203C28(Handle *);
extern void func_002084A0(Obj *);
void func_00208500(Obj *obj) {
    int done;
    Handle first,second;
    if(obj->xC0) {
        if(!obj->xC) {
            first=*obj->x0;
            func_00204080(&first,obj->x4!=0);
            func_00202E38(obj->x0,1,1,500.0f);
            obj->xC=1;
        } else {
            done=0;
            if(obj->x4) {
                first=*obj->x0;
                if(func_00203C28(&first)>0.9f) { done=1; goto finish; }
            }
            if(!obj->x4) {
                second=*obj->x0;
                if(func_00203C28(&second)<0.1f) done=1;
            }
finish:
            if((unsigned char)done) func_002084A0(obj);
        }
    }
}
