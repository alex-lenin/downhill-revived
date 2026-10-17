/* UNIT 0x00236700 0x002369D8 .sdata=0x29DC10:0x29DC38 .sbss=0x29E388:0x29E3A0 */
struct S { unsigned int x0, x4, x8; char pad[0x1c]; int x28; };

unsigned int D_0029DC10 = 0; /* next free sector */
unsigned int D_0029DC14 = 0; /* last sector */
int D_0029DC18 = 0;
int D_0029DC1C = 0;
int D_0029E388[2] __attribute__((section(".sbss"))) = { 0 };

/* read mode bytes passed to func_00202760 (sceCdRMode-like); defined at the end of
   the file, so the assembler builds the address with lui/addiu */
extern volatile unsigned char D_0029E390;

extern void func_002481C8(void);
extern void func_0025E790(int);
extern void func_00202820(int);
extern void func_00202910(void);
extern int func_00202760(unsigned int sector, int count, void *buf, void *arg);
extern int func_00248200(int sector, void *buf, int count, void *arg);

extern volatile unsigned char D_0029E391, D_0029E392;
extern unsigned char D_0029B301[];
extern int strlen(const char *);
extern char *strcpy(char *, const char *);
extern int func_00254D68(char *, const char *, ...);
extern int func_0020E520(void *, char *);
extern int func_0025E620(void);
extern int func_00247A40(int);
extern int func_0025E6A8(int);
extern void func_00248150(int,int,unsigned int);
extern void func_00248180(int,void *);
/* newlib-style ctype: D_0029B301 = _ctype_ + 1, bit 1 = lower case */
#define toupper(c) __extension__ ({ int __x = (c); (D_0029B301[__x]&2) ? __x - 'a' + 'A' : __x; })
/* strings below are this file's .sdata literals (D_0029DC20 "\\", D_0029DC28, D_0029DC30); D_0029E39x are volatile: their stores never land in delay slots */
int func_00236700(struct S *s, char *name) {
 char path[256]; char prefix[64]; char converted[256];
 int count=0, i, length, size;
 strcpy(prefix,"\\");
 length=strlen(name);
 for(i=0;i<length;i++) {
  if(name[i]=='/') converted[count++]='\\';
  else converted[count++]=toupper(name[i]);
 }
 converted[count]=0;
 func_00254D68(path,"%s%s%s",prefix,converted,";1");
 if(D_0029DC1C) {
  if(!func_0020E520(&s->x4,path)) return 0;
  D_0029DC10=s->x4;
  D_0029DC14=s->x4+(s->x8>>11)-1;
  s->x0=s->x8;
  D_0029E390=0;D_0029E391=0;D_0029E392=0;
  goto success;
 }
 if(!D_0029DC18) {
  while(func_0025E620()) {}
  func_00247A40(0);D_0029DC18=1;
 }
 size=func_0025E6A8(0x28010);
 s->x28=size;
 func_00248150(80,5,((unsigned int)(size+15)>>4)<<4);
 if(!func_0020E520(&s->x4,path)) return 0;
 s->x0=s->x8;
 D_0029E390=0;D_0029E391=0;D_0029E392=0;
 func_00248180(s->x4,(void *)&D_0029E390);
success:
 return 1;
}

int func_002368E8(struct S *s) {
    if (D_0029DC1C == 0) {
        func_002481C8();
        func_0025E790(s->x28);
    }
    return 1;
}

int func_00236928(int unused, void *buf, int size)
{
    int count;
    unsigned int left;

    if (D_0029DC1C != 0) {
        left = D_0029DC14 - D_0029DC10 + 1;
        count = size >> 11;
        if ((unsigned int)size >> 11 >= left) {
            count = left;
        }
        func_00202820(0);
        func_00202910();
        if (func_00202760(D_0029DC10, count, buf, (void *)&D_0029E390)) {
            D_0029DC10 += count;
            count <<= 11;
        }
    } else {
        count = func_00248200(size >> 11, buf, 1, D_0029E388) << 11;
    }
    return count;
}

volatile unsigned char D_0029E390 __attribute__((section(".sbss"))) = 0;
volatile unsigned char D_0029E391 __attribute__((section(".sbss"))) = 0;
volatile unsigned char D_0029E392 __attribute__((section(".sbss"))) = 0;
