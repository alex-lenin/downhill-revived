/* UNIT 0x00186B88 0x00186E10 .rodata=0x28C6E0:0x28C700 .sdata=0x29CD88:0x29CD90 .sbss=0x29DE28:0x29DE30 */
typedef struct Obj { char pad0[0xC8C]; char *xC8C; } Obj;
typedef struct Input { int x0,x4,x8,xC,x10; char pad14[0x8D]; char xA1[1]; } Input;
typedef struct Prefix { char text[9]; } Prefix;
extern const Prefix D_0028C6D0;
int D_0029CD88=0;
int D_0029DE28 __attribute__((section(".sbss")))=0;
int D_0029DE2C __attribute__((section(".sbss")))=0;
extern char D_003E3B80[],D_003D0F80[],D_0029CD90[];
extern unsigned char D_0029B301[];
extern unsigned int strlen(const char *);
extern char *strncat(char *,const char *,unsigned int);
extern char *func_00254544(char *,const char *);
extern int func_002460F8(int,int,char *,int),func_002464B8(int,char *,int),func_00246258(int);
extern void func_00184938(Input *,int);
extern int func_0020D930(char *,char *),func_002466A0(int,int *,int *);
static inline int Upper(int c) {
 int upper=c-32;
 if(!(D_0029B301[c]&2)) upper=c;
 return upper;
}
int func_00186B88(Obj *obj,Input *info,unsigned int *state) {
 Prefix prefix=D_0028C6D0;
 int status1,status2,status3;
 unsigned int index,len;
 char *name,*ch;
 switch(*state) {
 case 0:
 index=0;
 func_00254544(D_003E3B80,prefix.text);
 strncat(D_003E3B80,info->xA1,74);
 while(index<strlen(D_003E3B80)) {
  ch=D_003E3B80+index;index++;*ch=Upper(*ch);
 }
 strncat(D_003E3B80,D_0029CD90,2);
 D_0029CD88=func_0020D930(D_003D0F80,D_003E3B80);
 len=strlen(obj->xC8C);
 func_00254544(D_003E3B80,obj->xC8C);
 D_003E3B80[len]='/';
 func_00254544(D_003E3B80+1+len,info->xA1);
 func_002460F8(info->x10,info->xC,D_003E3B80,0x203);
 (*state)++;
 case 1:
 status1=2;
 if(!func_002466A0(1,&status1,&D_0029DE2C)) break;
 if(D_0029DE2C<0) goto error;
 goto advance;
 case 2:
 func_002464B8(D_0029DE2C,D_003D0F80,D_0029CD88);
 (*state)++;
 case 3:
 status2=6;
 if(!func_002466A0(1,&status2,&D_0029DE28)) break;
 if(D_0029DE28>=0) goto advance;
error:
 func_00184938(info,info->x4);
 return 1;
advance:
 (*state)++;
 break;
 case 4:
 func_00246258(D_0029DE2C);
 (*state)++;
 case 5:
 status3=3;
 if(func_002466A0(1,&status3,&D_0029DE28)) {
  if(D_0029DE28<0) func_00184938(info,info->x4);
  return 1;
 }
 break;
 }
 return 0;
}
