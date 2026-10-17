/* UNIT 0x0023C308 0x0023C3D8 */
typedef struct Word { char bytes[4]; } Word;
typedef struct Obj { char pad[0xa8]; int xA8; } Obj;
extern Word D_0029DCA0[];
extern char D_00663B58[];
extern void func_0023C1F0(Obj *,int *,int *,int *,int *);
extern int func_0023C7A8(int,int,int,int,Word *,int,int,int);
extern void func_0023C210(char *,int);
extern void func_0023BC60(void *);
int func_0023C308(Obj *obj) {
 Word data=D_0029DCA0[0];
 int a,b,c,d;
 int result;
 func_0023C1F0(obj,&a,&b,&c,&d);
 if(b+d<4) return 0;
 result=func_0023C7A8((a&0xfffffff)|0x20000000,b,(c&0xfffffff)|0x20000000,d,&data,4,0,0);
 func_0023C210(D_00663B58,result);
 func_0023BC60((char *)obj+0x48);
 if(!obj->xA8) obj->xA8=2;
 return 1;
}
