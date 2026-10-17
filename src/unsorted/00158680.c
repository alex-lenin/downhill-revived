/* UNIT 0x00158680 0x00158840 */
typedef struct { int x0,x4; } Context;
typedef struct { int count; int *objects[1]; } List;
typedef struct { char entries[0x1680]; int count; } World;
extern World D_004D0380;
extern int D_0029DA50;
extern char D_0029C6C8[];
extern void func_00238A68(int),func_00206878(void),func_00206A38(void),func_00207B40(void);
extern List *func_0020D170(World *,int);
extern void func_0020AC80(int,void *,int),func_002068E8(int *,int);
extern void func_001589A8(void),func_00158960(void);
extern void func_00209618(int **,void (*)(void),int *);
extern void func_0020A9A0(int,void (*)(void));
void func_00158680(Context *ctx,int unused,int flag) {
 int zeros[3]; int i=0,j,k; List *list; int **p,**q;
 D_0029DA50=0;
 func_00238A68(60);func_00206878();func_00206A38();
 for(;i<D_004D0380.count;i++) {
 list=func_0020D170(&D_004D0380,i);
 if(flag)func_0020AC80(i,D_0029C6C8,0);
 j=0;ctx->x0=*(int *)(D_004D0380.entries+i*72);ctx->x4=i;
 for(;j<list->count;j++){int *obj=list->objects[j];if((*obj&63)==7)func_002068E8(obj,i);}
 k=0;func_00207B40();
 for(;k<list->count;k++){if((*list->objects[k]&63)!=7){zeros[0]=0;zeros[1]=0;zeros[2]=0;func_00209618(&list->objects[k],func_001589A8,zeros);}}
 }
 func_0020A9A0(D_0029DA50,func_00158960);
}
