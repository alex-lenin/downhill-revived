/* UNIT 0x0012CF68 0x0012D248 .rodata=0x289000:0x289080 */
typedef struct Obj { char pad0[0x7ba8]; unsigned char x7ba8,x7ba9,x7baa,x7bab,x7bac,x7bad,x7bae,x7baf,x7bb0,x7bb1,x7bb2,x7bb3,x7bb4,x7bb5,x7bb6,x7bb7; int x7bb8; int x7bbc; int x7bc0,x7bc4,x7bc8; } Obj;
extern char D_0029C270[],D_0029C278[],D_0029C280[];
extern int D_004B2828[];
extern void func_0023D650(int,int *,int *);
extern int func_0023DED0(void);
extern char *func_001E6E80(int);
extern char *func_00254544(char *,const char *);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F518(int,int,int,int,int);
extern void func_0020F9F0(int,int,int,int,char *);
void func_0012CF68(Obj *obj,int arg) {
 char text[48],suffix[48];
 int width,height,center,y,extra;
 const char *unlocked,*locked,*yes,*no;
 if(obj->x7bb8) {
  if(obj->x7bc0<=0) { obj->x7bb8=0; return; } else {
   obj->x7bc0--;
   obj->x7bc8++;
   if(obj->x7bc8>=21) { obj->x7bc8=0; obj->x7bc4=!obj->x7bc4; }
   if(obj->x7bc4) {
    func_0023D650(arg,&width,&height);
    extra=0; center=width/2;
    y=72; if(func_0023DED0()==2) y=50;
    unlocked="Unlocked"; locked=D_0029C270; yes=D_0029C278; no=D_0029C280;
    func_00254544(text,func_001E6E80(obj->x7bb8));
    switch((unsigned int)obj->x7bb8-1) {
     case 0: extra=1; if(obj->x7bb3) func_00254544(suffix,unlocked); else func_00254544(suffix,locked); break;
     case 9: case 10: case 11: case 12: extra=1; func_00254544(suffix,unlocked); break;
     case 15: extra=1; if(obj->x7bb0) func_00254544(suffix,yes); else func_00254544(suffix,no); break;
     case 3: extra=1; if(obj->x7ba8) func_00254544(suffix,yes); else func_00254544(suffix,no); break;
     case 25: extra=1; if(obj->x7bb6) func_00254544(suffix,yes); else func_00254544(suffix,no); break;
     case 1: extra=1; if(obj->x7bad) func_00254544(suffix,yes); else func_00254544(suffix,no); break;
     case 5: extra=1; if(obj->x7ba9) func_00254544(suffix,yes); else func_00254544(suffix,no); break;
     case 4: extra=1; if(obj->x7baa) func_00254544(suffix,yes); else func_00254544(suffix,no); break;
     case 8: extra=1; if(obj->x7baf) func_00254544(suffix,yes); else func_00254544(suffix,no); break;
     case 7: extra=1; if(obj->x7bae) func_00254544(suffix,yes); else func_00254544(suffix,no); break;
     case 6: extra=1; if(obj->x7bab) func_00254544(suffix,yes); else func_00254544(suffix,no); break;
     case 18: extra=1; if(obj->x7bb2) func_00254544(suffix,yes); else func_00254544(suffix,no); break;
    }
    if(D_004B2828[0]>=2) func_0020FB80(0,352,256,-123,-32);
    else func_0020FB80(0,384,288,-123,-32);
    func_0020F518(0,140,140,140,140);
    func_0020F9F0(0,arg,center,y,text);
    if(extra) func_0020F9F0(0,arg,center,y+10,suffix);
   }
  }
 }
}
