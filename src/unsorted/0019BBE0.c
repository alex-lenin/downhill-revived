/* UNIT 0x0019BBE0 0x0019BD28 */
struct Obj { char pad[0x11]; signed char x11,x12,x13; char pad14[0x38]; int x4C; };
extern void func_0020F518(int,int,int,int,int);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F9F0(int,int,int,int,const char*);
extern void func_00254D68(char*,const char*,int);
extern char D_0029D178[],D_0029D180[],D_0029D188[],D_0029D190[],D_0028E5A8[],D_0029D198[],D_0029D1A0[];
void func_0019BBE0(struct Obj *o,int arg) {
 char buf[64];
 func_0020F518(0,110,110,200,170);
 func_0020FB80(0,256,192,-91,-32);
 if (!o->x13) {
 func_0020F9F0(0,arg,64,150,D_0029D178);
 func_0020F9F0(0,arg,64,160,D_0029D180);
 } else if (o->x4C) {
 func_0020F9F0(0,arg,64,150,D_0029D188);
 func_0020F9F0(0,arg,64,160,D_0029D180);
 } else {
 func_0020F9F0(0,arg,64,150,D_0029D190);
 func_0020F9F0(0,arg,64,160,D_0028E5A8);
 if(o->x11) func_00254D68(buf,D_0029D198,o->x12+1);
 else func_00254D68(buf,D_0029D1A0,o->x12);
 func_0020F9F0(0,arg,64,170,buf);
 }
}
