/* UNIT 0x00185590 0x00185730 .sbss=0x29DDFC:0x29DE04 */
typedef struct Source { char pad[0xC8C]; int xC8C; } Source;
typedef struct Params { char p0[12]; int xC,x10; char p14[0x8D]; char xA1[1]; } Params;
int D_0029DDFC __attribute__((section(".sbss")))=0;
int D_0029DE00 __attribute__((section(".sbss")))=0;
extern char D_003CA288[], D_003C9FC4[], D_0028C658[];
extern void func_00254544(char *,int);
extern int func_002460F8(int,int,char *,int),func_00246258(int);
extern int func_002466A0(int,int *,int *);
extern char *strncat(char *,const char *,unsigned int);
extern int strcmp(const char *,const char *);
int func_00185590(Source *s, Params *p, char *out, int *state) {
 int command;
 switch (*state) {
 case 0:
  func_00254544(D_003CA288,s->xC8C);
  strncat(D_003CA288,D_0028C658,74);
  func_002460F8(p->x10,p->xC,D_003CA288,3);
  (*state)++;
 case 1:
  command=2;
  if (func_002466A0(1,&command,&D_0029DDFC)) {
   if (D_0029DDFC==-1) goto abort;
   (*state)++;
  }
  break;
 case 2:
  func_00246258(D_0029DDFC);
  (*state)++;
 case 3:
  command=3;
  if (func_002466A0(1,&command,&D_0029DE00)) {
   if (D_0029DE00<0) {
abort:
    *out=0;
    return 1;
   }
   if (strcmp(D_003C9FC4,p->xA1) || strcmp(D_003C9FC4+64,p->xA1) || strcmp(D_003C9FC4+128,p->xA1)) *out=1;
   return 1;
  }
  break;
 default: return 0;
 }
 return 0;
}
