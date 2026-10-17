/* UNIT 0x00163970 0x00163A30 */
struct Obj { int x0; char pad4[0x3c]; int x40,x44; float x48,x4c,x50,x54,x58; int x5c,x60,x64,x68; unsigned char x6c,x6d,x6e; };
extern int func_0023CF58(int),func_0023CF78(int);
extern void func_00163D80(struct Obj *);
void func_00163970(struct Obj *obj,int arg1,int arg2) {
 int value;
 obj->x5c=0;obj->x0=arg2;obj->x6e=1;obj->x60=arg1;
 obj->x40=func_0023CF58(arg1);
 value=func_0023CF78(arg1);
 obj->x64=0;obj->x6d=0;obj->x44=value;
 obj->x48=1000000.0f;obj->x4c=0.4f;obj->x50=0.2f;obj->x54=0.5f;obj->x58=0.0008f;obj->x6c=1;
 func_00163D80(obj);
}
