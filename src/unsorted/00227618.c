/* UNIT 0x00227618 0x00227840 */
#include "gs_packet.h"
typedef struct Obj { char pad0[0x2750]; float x2750,x2754; char pad2758[0x70];float x27c8,x27cc,x27d0,x27d4;char pad27d8[0x3c];int x2814;char pad2818[12];int x2824;char pad2828[8];void *x2830,*x2834,*x2838,*x283c;char pad2840[20];int x2854; } Obj;
typedef struct Node {int x0;struct Node *x4;int x8;} Node;
typedef union Word {float f;int i;} Word;
typedef struct Entry {char pad[12];unsigned short xc;short xe;} Entry;
extern Entry D_0067DF90[];


extern int func_002249A8(int,int);
void func_00227618(Obj *obj) {
 Node *base,*next,*cur;Word *p;unsigned short n;int half;
 p=(Word *)(((unsigned int)obj+12)&0xfffffff0);base=(Node *)p;
 obj->x2854=42;
 p=(Word *)func_002246A0((unsigned int *)(p),obj->x2824,0);
 p[0].i=0x6c060190;
 p=(Word *)func_00224660((GsPacket *)(p+1),0,0x31,2,0,0,0,0,1);
 p[0].f=308.0f;p[1].f=3.0f;p[2].f=-1.5f;p[3].f=1023.0f;
 n=((Entry *)(obj->x2814*16+(unsigned int)D_0067DF90))->xc;
 p[5].i=9;half=(short)n>>1;
 p[4].i=half;p[6].i=half;p[7].i=(short)n-9;
 (Word *)func_00224660((GsPacket *)(p+8),0,0x535,3,0,obj->x2824!=256?0x156:0x116,1,0,0);
 p[12].f=obj->x27c8;p[13].f=obj->x27d0;p[14].f=obj->x2750;
 p[16].f=obj->x27cc-obj->x27c8;p[17].f=obj->x27d4-obj->x27d0;p[18].f=obj->x2754-obj->x2750;p[20].i=0;p[21].i=0;
 p+=23;obj->x2830=p;
 next=(Node *)((char *)p+0x540);base->x4=next;
 p=(Word *)next;next=(Node *)((char *)next+0x550);p[2].i=0;((Node *)p)->x4=next;
 p=(Word *)next;next=(Node *)((char *)next+0x550);p[2].i=0;((Node *)p)->x4=next;
 obj->x283c=(char *)next+16;
 p=(Word *)next;next=(Node *)((char *)next+0xc0);p[2].i=0;((Node *)p)->x4=next;
 p=(Word *)next;next=(Node *)((char *)next+0xc0);p[2].i=0;((Node *)p)->x4=next;
 p=(Word *)next;next=(Node *)((char *)next+0xc0);((Node *)p)->x4=0;p[2].i=0;
 p=(Word *)((char *)next+func_002249A8(obj->x2824,0)*4);
 obj->x2834=p;p=(Word *)((char *)p+0x7e0);obj->x2838=p;
}
