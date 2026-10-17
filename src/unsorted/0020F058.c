/* UNIT 0x0020F058 0x0020F240 */
typedef struct Env { short x0,x2,x4,x6; char pad8[8]; short x10,x12,x14,x16; int x18,x1C; } Env;
extern Env D_004F3570,D_004F3590;
extern char D_004F34B0[],D_004F3510[];
typedef struct Buf { char x[0x140]; } Buf;
typedef struct LargeBuf { char x[0x8000]; } LargeBuf;
extern Buf D_004F35C0[4]; extern LargeBuf D_004F3AC0[4];
extern int D_0029E188,D_0029E18C;
extern void *func_00243D80(void *,int,int,int,int,int,int,int);
extern void func_00210068(int,void *,void *,int),func_0020F5D0(int),func_0020F7D0(int);
void func_0020F058(int value,int mode) {
 int i,first,second; char *p,*q; Env *env;
 D_0029E188=mode;
 env=&D_004F3570;
 if(mode) { env->x0=512; env->x2=512; env->x4=512; env->x6=512; }
 else { env->x0=256; env->x2=256; env->x4=192; env->x6=240; }
 env->x10=0; env->x12=0; env->x14=0; env->x16=0x1400; env->x1C=0;
 env=&D_004F3590;
 if(mode) { env->x0=512; env->x2=512; env->x4=512; env->x6=512; }
 else { env->x0=256; env->x2=256; env->x4=192; env->x6=240; }
 env->x10=0; env->x12=0; env->x14=0; env->x16=0x1400; env->x1C=0;
 first=(short)((value-0x840)/64); second=(short)((value-64)/64);
 func_00243D80(D_004F3510,first,2,20,0,0,128,128);
 func_00243D80(D_004F34B0,second,1,0,0,0,8,2);
 for(i=0;i<4;i++) { func_00210068(i,&D_004F35C0[i],&D_004F3AC0[i],value); }
 func_0020F5D0(0); func_0020F7D0(0); func_0020F5D0(1); func_0020F7D0(1);
 D_0029E18C=1;
}
