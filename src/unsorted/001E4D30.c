/* UNIT 0x001E4D30 0x001E5190 .sdata=0x29D730:0x29D740 */
#include "scene.h"
#include "timer.h"
typedef unsigned int u128 __attribute__((mode(TI)));
typedef SceneNode Actor;
struct Input { char pad[0x40]; float matrix[16]; };
struct Global { char pad[0x501A8]; int count; };
extern struct Global D_002AF2C0;
int D_0029D730=0;
int *D_0029D734=0;
unsigned char D_0029D738=0;
int D_0029D73C=0;
extern Actor *D_004C5B80[], *D_004C5BD0[];
extern struct Input *D_004C5BF8[];
extern int D_004C5C20[];
extern char D_00759780[];
extern int func_001E4C10(u128 *, struct Input *, int);
extern Actor *func_00209108(void);
extern void func_002091A0(Actor *);
extern void func_00218278(Actor *, int);
extern void func_002182A8(Actor *, int);

void func_001E4D30(struct Input *input, int index) {
 u128 value;
 Actor *obj;
 int count;
 if (D_0029D73C < 20) {
  if (func_001E4C10(&value, input, index)) {
   obj = func_00209108();
   if (obj) {
    func_00218278(obj, D_0029D730);
    func_002182A8(obj, 1);
    *(u128 *)&obj->position = value;
    obj->flags &= ~((SceneNodeFlags)1 << 63);
    obj->flags &= ~((SceneNodeFlags)1 << 62);
    obj->flags &= ~((SceneNodeFlags)1 << 57);
    obj->x20 = 255.0f; obj->x24 = 255.0f; obj->x28 = 255.0f; obj->x2C = 255.0f;
    __asm__ volatile("lq $8,0(%4)\n\tlq $9,16(%4)\n\tlq $10,32(%4)\n\tlq $11,48(%4)\n\tsq $8,%0\n\tsq $9,%1\n\tsq $10,%2\n\tsq $11,%3" : "=m"(*(u128 *)&obj->matrix.row[0]), "=m"(*(u128 *)&obj->matrix.row[1]), "=m"(*(u128 *)&obj->matrix.row[2]), "=m"(*(u128 *)&obj->matrix.row[3]) : "r"(input->matrix) : "$8","$9","$10","$11","memory");
    count = D_0029D73C;
    {
     Actor **base1 = D_004C5BD0;
     Actor **base0 = D_004C5B80;
     base0[count] = obj;
     base1[index] = obj;
    }
    D_0029D73C = count + 1;
    obj->xB6 = index;
   }
  }
 }
}

unsigned char func_001E4E80(void) {
 unsigned char available[10]; int count,i,n,k; Actor **read,**write; Actor *a;
 count=D_002AF2C0.count;
 for(k=0;k<count;k++) available[k]=1;
 n=D_0029D73C;
 read=D_004C5B80; D_0029D738=0; write=D_004C5B80;
 for(;n!=0;n--,read++) {
 { Actor *first=*read;
 if(first) {
 first->xB8-=0.0625f;
 a=*read;
 if(a->xB8<=0.0f) {
 func_002182A8(a,0); func_00218278(*read,0); func_002091A0(*read);
 *read=0; D_0029D73C--;
 } else {
 int slot=a->xB6;
 *write=a; D_0029D738=1; available[slot]=0; write++;
 }
 }
 }
 }
 for(i=0;i<count;i++) {
 int ticks;
 if(available[i]) D_004C5BD0[i]=0;
 ticks=D_004C5C20[i];
 if(ticks>0) {
 D_004C5C20[i]=ticks-1;
 func_001E4D30(D_004C5BF8[i],i); D_0029D738=1;
 } else D_004C5BF8[i]=0;
 }
 return D_0029D738;
}

void func_001E5040(void) {
 int i;
 D_0029D738=0;
 D_0029D730=0;
 D_0029D734=0;
 D_0029D73C=0;
 for(i=19;i>=0;i--) D_004C5B80[i]=0;
 for(i=0;i<10;i++) { D_004C5BD0[i]=0; D_004C5C20[i]=0; D_004C5BF8[i]=0; }
}

void func_001E50C0(void)
{
    int *p = D_0029D734;

    if (p != 0) {
        *p = (*p & ~0x3F) | 1;
    }
    D_0029D734 = 0;
    D_0029D730 = 0;
}

void func_001E50E8(int *obj)
{
    int flags = *obj;
    int x20 = obj[8];

    D_0029D734 = obj;
    D_0029D730 = x20;
    *obj = (flags & ~0x3F) | 9;
}

void func_001E5110(struct Input *value, int index) {
    if (!D_0029D738) func_00236B98((TimerPool *)(D_00759780), (TimerCallbackNoArg)((void (*)(void))func_001E4E80), 1);
    D_004C5BF8[index] = value;
    D_004C5C20[index] = 10;
}
