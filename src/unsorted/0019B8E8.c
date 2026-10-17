/* UNIT 0x0019B8E8 0x0019BA40 */
typedef struct Obj { int x0; char pad[0x458]; int x45C; } Obj;
typedef struct Global { char pad[8]; int x8; } Global;
typedef struct Input { char pad[0x64]; int x64; } Input;
typedef struct Flags { char pad[0x24]; signed char x24[40]; } Flags;
extern Global D_00398C40;
extern Input D_003F52D0;
extern Flags D_0043DCC0;
extern char D_0043CD50[];
extern void func_001DE740(void *);
extern void func_00190358(void *);
extern int func_00190D60(void *,int,int);
extern int func_0019B0B8(void *,int);
extern void func_001DE730(void *,int);
extern void func_001DE8D0(void *);
extern void func_0018DC20(Obj *);
static inline void *Control(Global *g) { return &g->x8; }
static inline void *GetInput(Input *g) { return &g->x64; }
static inline signed char *GetFlags(Flags *g) { return g->x24; }
void func_0019B8E8(Obj *obj) {
 int i,count=0,selected=0; signed char *p; void *input;
 func_001DE740(Control(&D_00398C40));
 input=GetInput(&D_003F52D0);
 func_00190358(&D_003F52D0);
 if(func_00190D60(input,7,obj->x45C)) {
  i=0;p=GetFlags(&D_0043DCC0);
  do {
   if(!func_0019B0B8(D_0043CD50,i) && *p) {
    if(count==obj->x0) selected=i;
    count++;
   }
   i++;p++;
  } while(i<40);
  func_001DE730(Control(&D_00398C40),selected);
 } else if(func_00190D60(input,4,obj->x45C)||func_00190D60(input,2,obj->x45C)||func_00190D60(input,3,obj->x45C)) {
  func_001DE8D0(Control(&D_00398C40));
 }
 func_0018DC20(obj);
}
