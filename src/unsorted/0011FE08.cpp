/* UNIT 0x0011FE08 0x0011FF38 */
struct Handle {
 int x0,x4,x8,xC;
 void func_00203DC8(int);
 void func_00203E10(float);
 float func_00203C28();
 int func_00204230();
 void func_00202E38(int,int,float);
};
struct Obj {
 char pad0[0x6C]; Handle x6C;
 char pad7C[0x190]; float x20C;
 char pad210[0x14]; float x224;
 char pad228[0x36]; short x25E;
 Handle handle() { return x6C; }
};
extern void func_0011E860(Obj *);
extern void func_001220B0(Obj *,float);
void func_0011FE08(Obj *obj) {
 func_0011E860(obj);
 int flag = 0;
 obj->handle().func_00203DC8(1);
 obj->handle().func_00203E10(obj->x224);
 flag = obj->handle().func_00203C28() >= 1.0f || obj->handle().func_00204230() == 0;
 if((unsigned char)flag) obj->x6C.func_00202E38(1,1,250.0f);
 obj->x25E = 0;
 func_001220B0(obj,obj->x20C);
}
