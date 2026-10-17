/* UNIT 0x0020D728 0x0020D8F8 */
typedef unsigned int u32;
typedef struct { int x0; u32 x4; char x8[16]; char x18[16]; } Info;
extern int func_0020E520(Info *, char *);
extern void func_0020D028(char *, ...);
extern u32 func_0020E808(u32);
extern int func_00202820(int);
extern int func_00202760(int,u32,int,void *);
extern u32 func_00247CF8(void);
extern int func_00202910(void);
extern char D_00297EB8[],D_00297EF0[],D_00297F18[],D_00297F50[],D_00297F68[],D_00297F98[],D_004DA280[];
extern int D_0029E180;
u32 func_0020D728(char *name,int arg1) {
 Info info; int done; u32 count,now,last; int started;
 info.x0=0; info.x4=0; info.x8[0]=0; info.x18[0]=0;
 if(!func_0020E520(&info,name)) {func_0020D028(D_00297EF0,name,D_004DA280); return 0;}
 func_0020D028(D_00297EB8);
 func_0020D028(D_00297F18,name,info.x4,func_0020E808(info.x4),info.x0,arg1);
 func_00202820(0);
 do {
 if(func_00202760(info.x0,func_0020E808(info.x4),arg1,&D_0029E180)) {
 count=0; last=func_00247CF8();
 while(func_00202820(0)) {count++; now=func_00247CF8(); if(last+info.x4/10U<now) {last=func_00247CF8();func_0020D028(D_00297F50,last,info.x4,last*100U/info.x4);}}
 func_0020D028(D_00297F68,count);
 if(func_00202910()) {done=0;func_0020D028(D_00297F98,func_00202910());} else done=1;
 } else done=0;
 } while(!done);
 return info.x4;
}
