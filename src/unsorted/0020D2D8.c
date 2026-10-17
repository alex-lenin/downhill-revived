/* UNIT 0x0020D2D8 0x0020D4B0 */
typedef struct Entry { int x0,x4,x8; char padc[0x40-12]; int x40,x44; } Entry;
typedef struct Buffer { int x0,x4; } Buffer;
typedef struct State { Entry entries[80]; int x1680; Buffer *x1684; int x1688,x168c,x1690,x1694; } State;
extern char D_00297E40[],D_00297E68[],D_00297E90[];
extern int func_0020D660(State *,void *);
extern int func_0020D930(int,void *);
extern void func_0020D028(char *,...);
extern void func_0020D0F8(State *,int,int,void *,int);
extern void *memcpy(void *,const void *,unsigned int);
int func_0020D2D8(State *s,void *source,unsigned char flags) {
 int index=-1,addr,size;
 if(s->x1694) index=func_0020D660(s,source);
 addr=0; size=0;
 if(index<0) {
  if(s->x1684) { int n=(s->x1684->x4+127)&~127; s->x1684->x4=n+1; addr=n; }
  else addr=s->x1688;
  size=func_0020D930(addr,source);
  if(s->x1684) { int extra=size-1; s->x1684->x4=((s->x1684->x4+255)&~255)+extra; }
  func_0020D028(D_00297E40,source,addr,addr+size);
 } else {
  if(s->x1694==1) {
   addr=s->entries[index].x4;
   size=s->entries[index].x8;
   s->entries[s->x1680].x40=index;
   func_0020D028(D_00297E68,source,index);
  } else if(s->x1694==2) {
   size=s->entries[index].x8;
   if(s->x1684) { int n=(s->x1684->x4+127)&~127; s->x1684->x4=n+size; addr=n; }
   else addr=s->x1688;
   memcpy((void *)addr,(void *)s->entries[index].x4,size);
   func_0020D028(D_00297E90,source,index);
  }
 }
 func_0020D0F8(s,addr,size,source,flags);
 return addr;
}
