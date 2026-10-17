/* UNIT 0x0020D958 0x0020DB10 */
typedef unsigned int u32;
typedef struct { int x0; u32 x4; char x8[16]; char x18[16]; } Info;
extern char D_00297EB8[],D_00297F98[],D_00297FA8[],D_00297FC8[],D_00298010[],D_00298048[];
extern char D_00297F68[];
extern int D_0029E180;
extern int func_00202760(int,u32,int,void *),func_00202820(int),func_00202910(void),func_0020E520(Info *,char *);
extern u32 func_0020E808(u32);
extern void func_0020D028(char *,...);
u32 func_0020D958(char *arg0,int arg1,int arg2) {
 Info info; u32 addr; u32 size; int done; u32 sectors;
 info.x0=0; info.x4=0; info.x8[0]=0; info.x18[0]=0;
 if(!func_0020E520(&info,arg0)) {func_0020D028(D_00297FA8,arg0);return 0;}
 func_0020D028(D_00297EB8);
 size = ((u32)((arg2+1)<<17) < info.x4) ? 0x20000 : (info.x4-(arg2<<17));
 sectors=func_0020E808(0x20000);
 addr=info.x0+sectors*arg2;
 func_0020D028(D_00297FC8,info.x4,size,info.x0,addr);
 func_0020D028(D_00298010,arg0,info.x4,func_0020E808(size),addr,arg1);
 func_0020D028(D_00298048,size,info.x4);
 func_00202820(0);
 done=0;
 while(!done) {
  if(func_00202760(addr,func_0020E808(size),arg1,&D_0029E180)) {
   for(;;) {
    if(!func_00202820(0)) {
     func_0020D028(D_00297F68,0);
     break;
    }
   }
   done=1;
   if(func_00202910()) {
    done=0;
    func_0020D028(D_00297F98,func_00202910());
   }
  } else done=0;
 }
 return size;
}
