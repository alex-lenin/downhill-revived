/* UNIT 0x00202E00 0x002061F0 .rodata=0x297C50:0x297C90 .sbss=0x29E160:0x29E168 .sdata=0x29D994:0x29D9A0 */
#include "animation_query.h"
#if defined(__mips__)
#define CallShared00206E48_0 func_00206E48__c39
#else
#define CallShared00206E48_0 Forward00206E48_0
#endif
#include "animation_weight.h"
#include "scalar_api.h"
#include "include_asm.h"
#include "animation.h"
struct Count { unsigned short x0, x2; };
struct Metadata { char pad[16]; unsigned short x10; };
struct Node { int x0; struct Metadata *x4; char *x8,*xC; int x10,x14; float x18,x1C; char pad20[16]; int x30; float x34,x38; };
struct Object { char pad[24]; struct Node *x18; int x1C,x20; float x24,x28; };
struct Owner { char pad[12]; int xC; char pad10[16]; struct Count *x20; };
struct Arena { int x0, x4; };
extern struct Arena D_00663B08;
/* .sdata must be emitted in ascending address order (placed at 0x29D994 as one blob) */
int D_0029D994 = 3;
int D_0029D998 = 1;
struct Arena *D_0029D99C = &D_00663B08;

extern "C" void func_00204CE0(struct Node *,struct Owner *);
static inline char *allocate(int size) {
 char *result=(char *)((D_0029D99C->x4+15)&-16);
 D_0029D99C->x4=(int)(result+size);
 return result;
}
/* ---- merged from src/unsorted/00202E00.c ---- */
extern "C" {
struct Arena__c1 { int x0, x4; };
struct Header__c1 { unsigned short x0, count; };
struct Output__c1 { char pad[0xC]; int xC; };
struct Obj__c1 { char pad[0x20]; struct Header__c1 *x20; struct Output__c1 *x24; };
extern struct Arena__c1 D_00663B08__c1 asm("D_00663B08");
extern void func_00202E00__c1(struct Obj__c1 *obj) asm("func_00202E00");
void func_00202E00__c1(struct Obj__c1 *obj) {
    int start;
    struct Output__c1 *output;
    start = (D_0029D99C->x4 + 15) & ~15;
    output = obj->x24;
    D_0029D99C->x4 = start + obj->x20->count * 4;
    output->xC = start;
}
}
/* ---- merged from src/unsorted/00202E38.cpp ---- */
typedef struct Node__c2 Node__c2;
typedef AnimationHandle Arg__c2;
struct Node__c2 { int flags; unsigned char x4,x5; char pad6[14]; int x14; Node__c2 *x18; void *x1C; int x20; float x24; unsigned char x28; char pad29; short x2A; int x2C,x30; float x34,x38; int x3C,x40; float x44,x48,x4C; char pad50[8]; };
typedef struct { int flags; char pad4[16]; int x14; float x18,x1C; char pad20[10]; short x2A; char pad2C[16]; } Item__c2;
typedef struct { char pad0[8]; Node__c2 *x8; } Link__c2;
typedef struct { Node__c2 nodes[128]; unsigned char free[128]; signed char count; } Pool__c2;
extern Pool__c2 D_004CD480__c2 asm("D_004CD480");
/* D_0029D998 defined at file top (sdata order) */
extern int D_0029E160__c2 asm("D_0029E160");
extern Node__c2 *func_00206DD0__c2(Link__c2 *) asm("func_00206DD0");
extern void func_001FFB70__c2(int,Node__c2 *) asm("func_001FFB70");
/* Aggregate leaf aliases preserve retail copy scheduling (34 words differ
   with pointer calls, including through inline value wrappers). The PC
   declaration-only repair remains; the layout and dispatcher API are shared. */
extern void func_002038E0__c2(Arg__c2,int) asm("func_002038E0");
extern int func_00204170__c2(Arg__c2) asm("func_00204170");
extern void func_00203F40__c2(Arg__c2,int) asm("func_00203F40"),func_00203FE0__c2(Arg__c2,int) asm("func_00203FE0");
extern float func_00203C28__c2(Arg__c2) asm("func_00203C28");
extern void func_00203B68__c2(Arg__c2,int,float) asm("func_00203B68"),func_00203D18__c2(Arg__c2) asm("func_00203D18");
void func_00202E38(AnimationHandle *arg,int enable,int mode,float amount) {
    Node__c2 *node,*old; Item__c2 *item; Link__c2 *link; unsigned int kind; int mask;
    if(arg->x0 && arg->x4) {
        item=(Item__c2 *)arg->x0->x10+arg->xC;
        link=(Link__c2 *)arg->x0->x24;
        old=func_00206DD0__c2(link);
        if(mode==1 || mode==7) {
            if(!old || (arg->x0->x1C&2)) { func_002038E0__c2(*arg,enable!=0); }
            else if((Node__c2 *)arg->x4!=old || !old->x28) {
                if(D_004CD480__c2.count>=0) {
                    int index=(unsigned char)D_004CD480__c2.count++;
                    node=&D_004CD480__c2.nodes[D_004CD480__c2.free[index & 255]];
                } else node=0;
                if(node) func_001FFB70__c2(0x58,node);
                node->x5=mode;
                node->flags=(node->flags&~63)|34;
                if(enable) { if(func_00204170__c2(*arg)) func_00203F40__c2(*arg,0); else func_00203FE0__c2(*arg,0); }
                if(mode==7) { node->x4C=func_00203C28__c2(*arg); func_00203B68__c2(*arg,0,node->x4C); }
                func_00203D18__c2(*arg);
                kind=old->flags&63;
                if(kind==33) {
                    if(!old->x28) { old->x28=1; old->x14=D_0029E160__c2-old->x14; }
                    node->x20=old->x14; node->x24=*(float *)&old->x18; *(float *)&node->x28=*(float *)&old->x1C;
                } else if(kind==34) {
                    old->x14=(int)node;
                    node->x20=old->x30; node->x24=old->x34; *(float *)&node->x28=old->x38;
                }
                node->x18=old; node->x1C=item;
                node->flags=(node->flags&0x3FFFF)|(((unsigned int)item->flags>>18)<<18);
                mask=0x3FF80;
                node->flags=(node->flags&~mask)|(item->flags&mask);
                node->x44=amount; node->x48=D_0029E160__c2; node->x4=1;
                node->x40=0; node->x14=0;
                node->x30=item->x14; node->x34=item->x18; node->x38=item->x1C;
                item->x2A=0; link->x8=node; D_0029D998=0;
            }
        }
    }
}
INCLUDE_ASM("asm/nonmatchings/text", func_002031D8);
/* ---- merged from src/unsorted/002031E0.c ---- */
extern "C" {
typedef struct Source__c3 { int x0; char pad[16]; int x14; float x18,x1C; char pad20[10]; short x2A; } Source__c3;
typedef struct Node__c3 { int x0; unsigned char x4; signed char x5; short x6; int x8,xC,x10,x14,x18; Source__c3 *x1C; int x20; void *x24; short x28,x2A; int x2C,x30; float x34,x38; int x3C; float x40,x44,x48; int x4C,x50,x54; } Node__c3;
extern int D_0029E160__c3 asm("D_0029E160");
extern int func_00206D98__c3(void *,Node__c3 *) asm("func_00206D98");
extern void func_00206D18__c3(void *,Node__c3 *) asm("func_00206D18");
extern void func_00206C38__c3(void *,Node__c3 *) asm("func_00206C38");
void *func_002031E0(const AnimationHandle *input, void *output, int type, short priority, int apply, float value, float value2) {
    AnimationHandle h;
    Node__c3 *node=(Node__c3 *)output;
    Source__c3 *source;
    void *list;
    int flags;
    h = *input;
    if (!h.x0 || !h.x4) return 0;
    source = (Source__c3 *)h.x4;
    list = h.x0->x24;
    if (func_00206D98__c3(list,node)) func_00206D18__c3(list,node);
    if (apply) func_00203F40(&h,0);
    func_00203D18(&h);
    node->x5=type;
    node->x18=0;
    node->x0=(node->x0 & ~0x3F)|0x22;
    node->x1C=source;
    node->x0=(node->x0 & 0x3FFFF)|(((unsigned int)source->x0 >> 18)<<18);
    flags=source->x0 & 0x3FF80;
    node->x0=(node->x0 & 0xFFFC007F)|flags;
    node->x40=value;
    node->x44=value2;
    node->x48=(float)D_0029E160__c3;
    node->x4=1;
    node->x6=priority;
    node->x14=0;
    node->x50=0;
    node->x54=0;
    node->x30=source->x14;
    node->x34=source->x18;
    node->x38=source->x1C;
    source->x2A=0;
    func_00206C38__c3(list,node);
    return node;
}
}
/* ---- merged from src/unsorted/00203380.cpp ---- */
struct Node__c4 { char pad[0x24]; void *x24; };
struct Obj__c4 { int x0; unsigned char x4; };
extern int func_00206D18__c4(void *, void *) asm("func_00206D18");
void func_00203380(const AnimationHandle *input, void *output) {
    AnimationHandle handle=*input;
    Obj__c4 *p=(Obj__c4 *)output;
    if (handle.x0) {
        if (handle.x0->x24) func_00206D18__c4(handle.x0->x24, p);
        p->x4 = 0;
    }
}
/* ---- merged from src/unsorted/002033E0.c ---- */
extern "C" {
typedef struct Node__c5 { int x0; unsigned char x4; signed char x5; short x6; int x8, xC, x10, x14, x18; struct Node__c5 *x1C; int x20; void *x24; int x28,x2C,x30,x34,x38,x3C; float x40; int x44; float x48; int x4C,x50,x54; } Node__c5;
extern int D_0029E160__c5 asm("D_0029E160");
extern int func_00206D98__c5(void *, Node__c5 *) asm("func_00206D98");
extern void func_00206D18__c5(void *, Node__c5 *) asm("func_00206D18");
extern void func_00206C38__c5(void *, Node__c5 *) asm("func_00206C38");
void *func_002033E0(const AnimationHandle *input, void *output, int type, short priority, float value) {
    AnimationHandle h=*input;
    Node__c5 *node=(Node__c5 *)output;
    void *list;
    Node__c5 *source;
    int flags;
    if (!h.x0 || !h.x8) return 0;
    source=(Node__c5 *)h.x8;
    list = h.x0->x24;
    if (func_00206D98__c5(list,node)) func_00206D18__c5(list,node);
    node->x5=type;
    node->x18=0;
    node->x0=(node->x0 & ~0x3F)|0x22;
    node->x1C=source;
    node->x0=(node->x0 & 0x3FFFF)|(((unsigned int)source->x0 >> 18)<<18);
    flags=source->x0 & 0x3FF80;
    node->x0=(node->x0 & 0xFFFC007F)|flags;
    node->x40=value;
    node->x6=priority;
    node->x44=0;
    node->x14=0;
    node->x50=0;
    node->x54=0;
    node->x30=0;
    node->x34=0;
    node->x38=0;
    node->x48=(float)D_0029E160__c5;
    node->x4=1;
    func_00206C38__c5(list,node);
    return node;
}
}
/* ---- merged from src/unsorted/00203540.c ---- */
extern "C" {
typedef struct { unsigned short count; char pad[14]; unsigned int *items[1]; } List__c6;
extern int func_00203540__c6(List__c6 *list, unsigned int id) asm("func_00203540");
int func_00203540__c6(List__c6 *list, unsigned int id) {
    int i = 0;
    if (list->count) {
        int limit = list->count;
        unsigned int **p = list->items;
        do {
            if (*p && (**p >> 18) == id) return i;
            i++;
            p++;
        } while (i < limit);
    }
    return -1;
}
}
/* ---- merged from src/unsorted/00203590.c ---- */
extern "C" {
typedef struct Entry__c7 { unsigned int x0; char pad4[0xc]; char *x10; char pad14[0xc]; void *x20; } Entry__c7;
typedef struct Group__c7 { int x0; int x4; Entry__c7 *x8[1]; } Group__c7;

typedef struct Global__c7 { char pad[0x1680]; int x1680; } Global__c7;
extern Global__c7 D_004D0380__c7 asm("D_004D0380");
extern Group__c7 *D_004D0108__c7[] asm("D_004D0108");
extern int func_00203540__c7(void *, int) asm("func_00203540");

void func_00203590(AnimationHandle *arg0, int arg1, int arg2, int arg3) {
 int i, count, index;
 Group__c7 **group;
 Entry__c7 **entry;
 arg0->x0 = 0;
 arg0->x4 = 0;
 arg0->xC = -1;
 for (i = 0; i < D_004D0380__c7.x1680; i++) {
 group = &D_004D0108__c7[i];
  if (*group) {
   count = (*group)->x4;
   entry = (*group)->x8;
   while (count) {
    if (((*entry)->x0 >> 18) == arg1 && (((*entry)->x0 >> 7) & 0x7ff) == arg2) {
     index = func_00203540__c7((*entry)->x20, arg3);
     if (index >= 0) {
      Entry__c7 *found = *entry;
      arg0->x0 = (AnimationOwner *)found;
      arg0->x4 = (AnimationTrack *)(found->x10 + index * 0x3c);
      arg0->xC = index;
      return;
     }
    }
    count--;
    entry++;
   }
  }
 }
}
}
/* ---- merged from src/unsorted/002036C8.c ---- */
extern "C" {
typedef struct Entry__c8 {
    unsigned int bits;
    char pad4[0xC];
    char *x10;
    char pad14[0xC];
    void *x20;
} Entry__c8;

typedef struct { int x0, count; Entry__c8 *entries[1]; } List__c8;
extern List__c8 *D_004D0108__c8[] asm("D_004D0108");
extern int func_00203540__c8(void *list, int id) asm("func_00203540");


void func_002036C8(AnimationHandle *out, int first, int second, int id, int index) {
    List__c8 *list;
    Entry__c8 **p;
    Entry__c8 *entry;
    unsigned int bits;
    int count;
    int found;

    out->xC = -1;
    out->x0 = 0;
    out->x4 = 0;
    list = D_004D0108__c8[index];
    if (list) {
        count = list->count;
        p = list->entries;
        while (count) {
            bits = (*p)->bits;
            if ((bits >> 18) == first && ((bits >> 7) & 0x7FF) == second) {
                found = func_00203540__c8((*p)->x20, id);
                if (found >= 0) {
                    entry = *p;
                    out->x0 = (AnimationOwner *)entry;
                    out->x4 = (AnimationTrack *)(entry->x10 + found * 0x3C);
                    out->xC = found;
                    return;
                }
            }
            count--;
            p++;
        }
    }
}
}
INCLUDE_ASM("asm/nonmatchings/text", func_002037C8);
/* ---- merged from src/unsorted/002037D0.c ---- */
extern "C" {

typedef struct { int x0,count; unsigned int *entries[1]; } List__c9;
extern List__c9 *D_004D0108__c9[] asm("D_004D0108");

void func_002037D0(AnimationHandle *out,int first,int second,int index) {
 List__c9 *list; unsigned int **p,*entry,bits; int count;
 out->xC=-1; out->x0=0; out->x4=0;
 list=D_004D0108__c9[index];
 if(list) {
 count=list->count; p=list->entries;
 while(count) {
 entry=*p; bits=*entry;
 if((bits>>18)==first && ((bits>>7)&0x7FF)==second) { out->x0=(AnimationOwner *)entry; return; }
 count--; p++;
 }
 }
}
}
/* ---- merged from src/unsorted/00203848.c ---- */
extern "C" {

int D_0029E160[2];
float func_00203848(const AnimationHandle *input) {
    AnimationHandle handle = *input;
    AnimationHandle *p = &handle;
 float value;
 if (!p->x0 || !p->x4) return 0.0f;
 if (p->x4->x28) value = p->x4->x1C * (float)(D_0029E160[0] - p->x4->x14);
 else value = (float)p->x4->x14 * p->x4->x1C;
 return value + p->x4->x18;
}
}
INCLUDE_ASM("asm/nonmatchings/text", func_002038D8);
/* ---- merged from src/unsorted/002038E0.c ---- */
extern "C" {

void func_002038E0(const AnimationHandle *input, int force)
{
    AnimationHandle h = *input;
    AnimationHandle *p = &h;
    AnimationRange *limits;
    float value;

    if (p->x0) {
        limits = p->x0->x20->entries[p->xC];
        func_00206320(p);
        func_00203D18(p);
        func_00204080(p, 1);
        value = func_00203848(p);
        if (force || value < limits->x4) {
            func_00203F40(p, 0);
        }
        func_00204450(p, 0);
    }
}
}
/* ---- merged from src/unsorted/002039B0.c ---- */
extern "C" {

void func_002039B0(const AnimationHandle *input, int force)
{
    AnimationHandle h = *input;
    AnimationHandle *p = &h;
    AnimationRange *limits;
    float value;

    if (p->x0) {
        limits = p->x0->x20->entries[p->xC];
        func_00206320(p);
        func_00203D18(p);
        func_00204080(p, 0);
        value = func_00203848(p);
        if (force || value > limits->x8) {
            func_00203FE0(p, 0);
        }
        func_00204450(p, 0);
    }
}
}
/* ---- merged from src/unsorted/00203A80.c ---- */
extern "C" {

extern int D_0029E160__c13 asm("D_0029E160");
extern void func_002063F0__c13(AnimationRange *, AnimationOwner *) asm("func_002063F0");

void func_00203A80(const AnimationHandle *handle, int mode, float value) {
    AnimationHandle local = *handle;
    AnimationRange *entry;
    int time;
    AnimationHandle *p = &local;

    if (p->x0 && p->x4) {
        entry = p->x0->x20->entries[p->xC];
        func_00203D18(p);
        time = D_0029E160__c13;
        p->x4->x14 = time;
        if (entry->x8 < value) {
            p->x4->x18 = entry->x8;
        } else if (value < entry->x4) {
            p->x4->x18 = entry->x4;
        } else {
            p->x4->x18 = value;
        }
        if (mode) func_002063F0__c13(entry, p->x0);
        func_00203CB8(p);
    }
}
}
/* ---- merged from src/unsorted/00203B68.c ---- */
extern "C" {

extern int D_0029E160__c14 asm("D_0029E160");

extern void func_00203D18__c14(const AnimationHandle *, struct AnimationOwner *) asm("func_00203D18");
extern void func_002063F0__c14(struct AnimationRange *, struct AnimationOwner *) asm("func_002063F0");

#if defined(__mips__)
void func_00203B68(const AnimationHandle *src, int apply, float t)
#else
void SharedCall00203B68(const struct AnimationHandle * src_view, int apply, float t)
#endif
{
#if !defined(__mips__)
    const AnimationHandle * src = (const AnimationHandle *)src_view;
#endif

    struct AnimationHandle h;
    struct AnimationRange *range;
    float min, max;

    h = *src;
    if (h.x0 != 0 && h.x4 != 0) {
        range = h.x0->x20->entries[h.xC];
        min = range->x4;
        max = range->x8;
        h.x4->x14 = h.x4->x28 ? D_0029E160__c14 : 0;
        h.x4->x18 = (max - min) * t + min;
        if (apply != 0) {
            if (h.x4->x28 == 0) {
                func_00203D18__c14(&h, h.x0);
            }
            func_002063F0__c14(range, h.x0);
        }
    }
}
}
/* ---- merged from src/unsorted/00203C28.c ---- */
extern "C" {

float func_00203C28(const AnimationHandle *input) {
    AnimationHandle handle = *input;
 float result = -1.0f;
 if (handle.x0) {
  struct AnimationRange *range = handle.x0->x20->entries[handle.xC];
  float delta = range->x8 - range->x4;
  result = (func_00203848(&handle) - range->x4) / delta;
 }
 return result;
}
}
/* ---- merged from src/unsorted/00203CB8.cpp ---- */

extern int D_0029E160__c16 asm("D_0029E160");
void func_00203CB8(const AnimationHandle *input) {
    AnimationHandle handle = *input;
    if (handle.x0 && handle.x4 && handle.x4->x28) {
        handle.x4->x14 = D_0029E160__c16 - handle.x4->x14;
        handle.x4->x28 = 0;
    }
}
/* ---- merged from src/unsorted/00203D18.c ---- */
extern "C" {

extern int D_0029E160__c17 asm("D_0029E160");
void func_00203D18(const AnimationHandle *handle) {
 AnimationHandle local = *handle;
 if (local.x0 && local.x4) {
  if (!local.x4->x28) {
   local.x4->x14 = D_0029E160__c17 - local.x4->x14;
   local.x4->x28 = 1;
  }
  if (!local.x0->x24) {
   if (func_00204170(&local)) func_002038E0(&local, 0);
   else func_002039B0(&local, 0);
  }
 }
}
}
/* ---- merged from src/unsorted/00203DC8.c ---- */
extern "C" {

void func_00203DC8(const AnimationHandle *arg0, int value) {
    struct AnimationHandle copy = *arg0;
    if (copy.x0 && copy.x4) copy.x4->x29 = value;
}
}
/* ---- merged from src/unsorted/00203E10.c ---- */
extern "C" {

extern int D_0029E160__c19 asm("D_0029E160");
void func_00203E10(const AnimationHandle *input,float speed) {
 struct AnimationHandle local=*input;
 struct AnimationHandle *p=&local;
 float magnitude=__builtin_fabsf(speed);
 float value,old;
 int timer;
 struct AnimationTrack *node;
 if(local.x0 && local.x4) {
  value=func_00203848(&local);
  timer=D_0029E160__c19;
  if(local.x4->x28==0) {
   local.x4->x18=value; local.x4->x14=0;
  } else {
   local.x4->x18=value; local.x4->x14=timer;
  }
  node=p->x4; old=node->x1C;
  if(old==0.0f) { node->x1C=speed; return; }
  value=magnitude;
  if(old<=0.0f) value=-value;
  node->x1C=value;
 }
}
}
/* ---- merged from src/unsorted/00203EF0.cpp ---- */

void func_00203EF0(const AnimationHandle *input) {
    AnimationHandle handle = *input;
    if (handle.x0) func_00203E10(&handle, 1.0f);
}
/* ---- merged from src/unsorted/00203F40.c ---- */
extern "C" {

extern int D_0029E160__c21 asm("D_0029E160");
void func_00203F40(const AnimationHandle *handle, int mode) {
 AnimationHandle local = *handle;
 AnimationRange *entry;
 AnimationHandle *p = &local;
 if (p->x0 && p->x4) {
  entry = p->x0->x20->entries[p->xC];
  if (mode) func_00203A80(p, 1, entry->x4);
  else {
   if (p->x4->x28) p->x4->x14 = D_0029E160__c21;
   else p->x4->x14 = 0;
   p->x4->x18 = entry->x4;
  }
 }
}
}
/* ---- merged from src/unsorted/00203FE0.c ---- */
extern "C" {

extern int D_0029E160__c22 asm("D_0029E160");
void func_00203FE0(const AnimationHandle *handle, int mode) {
 AnimationHandle local = *handle;
 AnimationRange *entry;
 AnimationHandle *p = &local;
 if (p->x0 && p->x4) {
  entry = p->x0->x20->entries[p->xC];
  if (mode) func_00203A80(p, 1, entry->x8);
  else {
   if (p->x4->x28) p->x4->x14 = D_0029E160__c22;
   else p->x4->x14 = 0;
   p->x4->x18 = entry->x8;
  }
 }
}
}
/* ---- merged from src/unsorted/00204080.c ---- */
extern "C" {

extern int D_0029E160__c23 asm("D_0029E160");

void func_00204080(const AnimationHandle *input, int flag)
{
    AnimationHandle h = *input;
    AnimationHandle *p = &h;
    unsigned int now;
    float value;
    AnimationTrack *anim;
    float elapsed;

    if (p->x0 == 0 || p->x4 == 0) {
        return;
    }
    now = D_0029E160__c23;
    elapsed = (float)(now - (unsigned int)p->x4->x14);
    anim = p->x4;
    value = elapsed * anim->x1C + anim->x18;
    if (flag) {
        if (!(anim->x1C < 0.0f)) {
            return;
        }
    } else if (!(0.0f < anim->x1C)) {
        return;
    }
    if (anim->x28) {
        anim->x18 = value;
        p->x4->x14 = now;
    }
    p->x4->x1C = -p->x4->x1C;
}
}
/* ---- merged from src/unsorted/00204170.c ---- */
extern "C" {

int func_00204170(const AnimationHandle *p) {
    struct AnimationHandle ref=*p;
    if (!ref.x0 || !ref.x4 || !(ref.x4->x1C < 0.0f)) return 1;
    return 0;
}
}
/* ---- merged from src/unsorted/002041D0.c ---- */
extern "C" {

float func_002041D0(const AnimationHandle *p) {
    struct AnimationHandle ref=*p;
    float result;
    if (!ref.x0) result=0.0f;
    else result=ref.x0->x20->entries[(short)ref.xC]->x8;
    return result;
}
}
/* ---- merged from src/unsorted/00204230.c ---- */
extern "C" {

int func_00204230(const AnimationHandle *arg0)
{
    AnimationHandle h = *arg0;

    if (h.x0 != 0 && h.x4 != 0 && h.x4->x28 != 0) {
        return 1;
    }
    return 0;
}
}
/* ---- merged from src/unsorted/00204280.c ---- */
extern "C" {
float func_00204280(const AnimationHandle *input) {
    AnimationHandle state = *input;
    float result = 0.0f;
    if (state.x0 && state.x4) {
        AnimationRange *values = state.x0->x20->entries[state.xC];
        result = (values->x8 - values->x4) / state.x4->x1C;
    }
    return result;
}
}
/* ---- merged from src/unsorted/002042F0.c ---- */
extern "C" {
float func_002042F0(const AnimationHandle *arg, float scale) {
    AnimationHandle local = *arg;
    float value = -1.0f;
    if (local.x0) {
        AnimationRange *values = local.x0->x20->entries[local.xC];
        value = (values->x8 - values->x4) * scale;
    }
    return value;
}
}
/* ---- merged from src/unsorted/00204358.c ---- */
extern "C" {
typedef struct Obj__c29 {
 char pad0[0x1C];
 unsigned int x1C_0 : 4;
 unsigned int x1C_4 : 4;
 unsigned int x1C_8 : 4;
} Obj__c29;

extern char D_004D0248__c29[] asm("D_004D0248");
extern int func_00202BD0__c29(void *, int) asm("func_00202BD0");
extern void func_00202BA0__c29(void *, int, int, int) asm("func_00202BA0");

extern void func_00204358__c29(Obj__c29 **handle, int value) asm("func_00204358");
void func_00204358__c29(Obj__c29 **handle, int value) {
 int index, max;
 int count;
 if (*handle != 0) {
  max = 5;
  index = value < max ? value : max;
  if (index != (*handle)->x1C_4) {
   count = func_00202BD0__c29(D_004D0248__c29, index);
   func_00202BA0__c29(D_004D0248__c29, index, count, 1);
   func_00202BA0__c29(D_004D0248__c29, (*handle)->x1C_4, (*handle)->x1C_8, -1);
   (*handle)->x1C_4 = index;
   (*handle)->x1C_8 = count;
  }
 }
}
}
INCLUDE_ASM("asm/nonmatchings/text", func_00204448);
/* ---- merged from src/unsorted/00204450.c ---- */
extern "C" {

void func_00204450(const AnimationHandle *arg0, unsigned short value) {
    struct AnimationHandle copy = *arg0;
    if (copy.x0 && copy.x4) copy.x4->x2A = value;
}
}
INCLUDE_ASM("asm/nonmatchings/text", func_00204498);
/* ---- merged from src/unsorted/002044A0.c ---- */
extern "C" {
typedef struct Flags__c31 {
    char pad0[0x8];
    unsigned char *bits;
} Flags__c31;

typedef struct Item__c31 {
    char pad0[0x12];
    short groupA[3];
    short groupB[3];
    char pad1E[0xC0 - 0x1E];
    long state;
} Item__c31;

#define BIT(f, idx) (((f)->bits[(unsigned)(idx) >> 3] >> ((idx) & 7)) & 1)

extern void func_002044A0__c31(Flags__c31 *flags, Item__c31 *item) asm("func_002044A0");
void func_002044A0__c31(Flags__c31 *flags, Item__c31 *item)
{
    int on;
    int idx;

    on = 0;
    idx = item->groupA[0];
    if (idx >= 0)
        on = BIT(flags, idx);
    idx = item->groupA[1];
    if (idx >= 0)
        on |= BIT(flags, idx);
    idx = item->groupA[2];
    if (idx >= 0)
        on |= BIT(flags, idx);
    if (on)
        item->state |= 2;

    on = 0;
    idx = item->groupB[0];
    if (idx >= 0)
        on = BIT(flags, idx);
    idx = item->groupB[1];
    if (idx >= 0)
        on |= BIT(flags, idx);
    idx = item->groupB[2];
    if (idx >= 0)
        on |= BIT(flags, idx);
    if (on)
        item->state |= 1;
}
}
/* ---- merged from src/unsorted/002045C8.c ---- */
extern "C" {
typedef struct BitSet__c32 {
    char pad0[0x8];
    unsigned char *x8;
} BitSet__c32;

typedef struct Item__c32 {
    char pad0[0x12];
    short x12[6];
} Item__c32;

static inline void ClearBit(BitSet__c32 *set, unsigned int bit) {
    set->x8[bit >> 3] &= ~(1 << (bit & 7));
}

extern void func_002045C8__c32(BitSet__c32 *set, Item__c32 *item) asm("func_002045C8");
void func_002045C8__c32(BitSet__c32 *set, Item__c32 *item) {
    if (item->x12[0] >= 0) {
        ClearBit(set, item->x12[0]);
    }
    if (item->x12[1] >= 0) {
        ClearBit(set, item->x12[1]);
    }
    if (item->x12[2] >= 0) {
        ClearBit(set, item->x12[2]);
    }
    if (item->x12[3] >= 0) {
        ClearBit(set, item->x12[3]);
    }
    if (item->x12[4] >= 0) {
        ClearBit(set, item->x12[4]);
    }
    if (item->x12[5] >= 0) {
        ClearBit(set, item->x12[5]);
    }
}
}
/* ---- merged from src/unsorted/002046F8.c ---- */
extern "C" {
typedef struct { unsigned short key,value; } Entry__c33;
typedef struct { int last; Entry__c33 entries[1]; } Table__c33;
extern int func_002046F8__c33(int key,Table__c33 *table) asm("func_002046F8");
int func_002046F8__c33(int key,Table__c33 *table) {
 int lo,hi,mid; unsigned short found;
 if(table) {
 hi=table->last; lo=0;
 while(lo<=hi) {
 mid=(lo+hi)/2;
 found=table->entries[mid].key;
 if(found<key) lo=mid+1;
 else if(key<found) hi=mid-1;
 else return table->entries[mid].value;
 }
 }
 return -1;
}
}
/* ---- merged from src/unsorted/00204770.cpp ---- */

float func_00204770(AnimationHandle *h) {
    /* Each retail aggregate call copies the handle to its own stack temporary. */
    AnimationHandle copy;
    float value=(copy = *h, func_00203848(&copy));
    AnimationTrack *track=h->x4;
    AnimationRange *limits=track->x4;
    if(value>=track->x4->x8 && track->x1C>0.0f) {
        if(track->x29==0) {
            if(h->x0->x24->x8==track) { (copy = *h, func_00203CB8(&copy)); }
            (copy = *h, func_00203FE0(&copy,0));
            value=(copy = *h, func_00203848(&copy));
        } else {
            track->x2A++;
            (copy = *h, func_00203A80(&copy,0,value-track->x4->x8));
            (copy = *h, func_00203D18(&copy));
            value=(copy = *h, func_00203848(&copy));
        }
    } else if(value<track->x4->x4 && track->x1C<0.0f) {
        if(track->x29==0) {
            if(h->x0->x24->x8==track) { (copy = *h, func_00203CB8(&copy)); }
            (copy = *h, func_00203F40(&copy,0));
            value=(copy = *h, func_00203848(&copy));
        } else {
            track->x2A++;
            (copy = *h, func_00203FE0(&copy,0));
            value=(copy = *h, func_00203848(&copy));
        }
    } else if(value<track->x4->x4 && track->x29!=0) {
        track->x2A++;
        (copy = *h, func_00203F40(&copy,0));
        value=(copy = *h, func_00203848(&copy));
    }
    return value;
}
/* ---- merged from src/unsorted/00204A98.c ---- */
extern "C" {
typedef struct Node__c35 {
    unsigned short x0;
    unsigned short x2;
    char pad4[0x8];
    unsigned short xC;
    char padE[0x2];
    unsigned short x10;
    unsigned short x12[1];
} Node__c35;

typedef struct Obj__c35 {
    int x0;
    Node__c35 *x4;
    float *x8;
    unsigned char *xC;
    char pad10[0x20];
    unsigned short *x30;
} Obj__c35;

typedef struct Arg__c35 {
    char pad0[0x20];
    Node__c35 *x20;
} Arg__c35;

typedef struct Context__c35 {
    Arg__c35 *x0;
    Obj__c35 *x4;
    int x8;
    unsigned short xC;
} Context__c35;

extern float func_00204770__c35(Context__c35 *ctx) asm("func_00204770");
extern float func_00206E48__c35(Node__c35 *node, unsigned short *values, float scale) asm("func_00206E48");

extern void func_00204A98__c35(Obj__c35 *obj, Arg__c35 *arg) asm("func_00204A98");
void func_00204A98__c35(Obj__c35 *obj, Arg__c35 *arg)
{
    Context__c35 ctx;
    Node__c35 *node;
    unsigned short *indices;
    void *ptr; /* first node, then reused as the value cursor */
    unsigned char *bits;
    unsigned char *clear;
    float *output;
    float scale;
    int bytes;
    int count;
    unsigned int index;

    ptr = obj->x4;
    bytes = (arg->x20->x2 + 7) >> 3;
    clear = obj->xC;
    while (bytes) {
        *clear++ = 0;
        bytes--;
    }
    /* do/while(0) block: needed for the retail register allocation */
    do {
        ctx.x0 = arg;
        ctx.x8 = 0;
        ctx.x4 = obj;
        indices = ((Node__c35 *)ptr)->x12;
        ctx.xC = ((Node__c35 *)ptr)->x10;
        node = (Node__c35 *)ptr;
        scale = func_00204770__c35(&ctx);
        count = node->xC;
    } while (0);
    bits = obj->xC;
    ptr = obj->x30;
    output = obj->x8;
    while (count) {
        count--;
        node = (Node__c35 *)((char *)node - *indices++ * 4);
        index = node->x2;
        output[index] = func_00206E48__c35(node, (unsigned short *)ptr, scale);
        bits[index >> 3] |= 1 << (index & 7);
        if ((unsigned int)((node->x0 & 7) - 2) >= 2) {
            ptr = (unsigned short *)ptr + 1;
        }
    }
}
}
/* ---- merged from src/unsorted/00204BE8.c ---- */
extern "C" {
typedef struct Header__c36 { unsigned short x0, x2; } Header__c36;
typedef struct Input__c36 { char pad[0x20]; Header__c36 *x20; } Input__c36;
typedef struct Object__c36 Object__c36;
struct Object__c36 { int x0; void (*x4)(Object__c36 *, Input__c36 *, char *); char pad[8]; char *x10; };
extern void func_00204BE8__c36(Object__c36 *obj, Input__c36 *input) asm("func_00204BE8");
void func_00204BE8__c36(Object__c36 *obj, Input__c36 *input) {
 int count = (input->x20->x2 + 7) >> 3;
 char *p = obj->x10;
 if (count != 0) {
  do { *p = 0; count--; p++; } while (count != 0);
 }
 obj->x4(obj, input, p);
}
}
/* ---- merged from src/unsorted/00204C40.c ---- */
extern "C" {
struct Object__c37 { int flags, x4, x8, xC, x10, x14; };
struct Input__c37 { int x0, x4, x8, xC; };
extern void func_00204CE0__c37(struct Object__c37 *, struct Input__c37 *) asm("func_00204CE0");
extern void func_00204C40__c37(struct Object__c37 *obj, struct Input__c37 *input) asm("func_00204C40");
void func_00204C40__c37(struct Object__c37 *obj, struct Input__c37 *input) {
 int type = obj->flags & 63;
 if (type == 33) {
  obj->x8 = input->x4; obj->xC = input->x8; obj->x10 = input->xC;
  func_00204CE0__c37(obj, input);
 } else if (type == 34) {
  obj->x8 = input->x4; obj->xC = input->x8; obj->x10 = input->xC;
  func_00204CE0__c37(obj, input);
 } else if (type == 43) {
  obj->xC = input->x4; obj->x10 = input->x8; obj->x14 = input->xC;
  func_00204CE0__c37(obj, input);
 }
}
}
struct ArenaStack__web6 { int x0, x4, x8; };
extern "C" void func_00204A98__web6(struct Node *, struct Owner *) asm("func_00204A98");
extern "C" void func_00204BE8__web6(struct Node *, struct Owner *) asm("func_00204BE8");
extern "C" void func_00204F38__web6(struct Node *, struct Owner *) asm("func_00204F38");
extern "C" void func_002053B0__web6(struct Node *, struct Owner *) asm("func_002053B0");
extern "C" void func_002055D0__web6(struct Node *, struct Owner *) asm("func_002055D0");
extern "C" void func_00205850__web6(struct Node *, struct Owner *) asm("func_00205850");
extern "C" void func_00205AA0__web6(struct Node *, struct Owner *) asm("func_00205AA0");
extern "C" void func_00205D08__web6(struct Node *, struct Owner *) asm("func_00205D08");
extern "C" void func_00204CE0(struct Node *node, struct Owner *owner) {
    switch ((unsigned int)node->x0 & 63) {
    case 33:
        func_00204A98__web6(node, owner);
        break;
    case 43:
        func_00204BE8__web6(node, owner);
        break;
    case 34: {
        {
            struct ArenaStack__web6 *arena = (struct ArenaStack__web6 *)D_0029D99C;
            int p = (arena->x4 + 3) & ~3;
            int old = arena->x8;
            arena->x8 = p;
            arena->x4 = p + 4;
            *(int *)p = old;
        }
        switch (((unsigned char *)node)[5]) {
        case 1:
        case 7: func_00204F38__web6(node, owner); break;
        case 2: func_002053B0__web6(node, owner); break;
        case 3: func_002055D0__web6(node, owner); break;
        case 4: func_00205850__web6(node, owner); break;
        case 5: func_00205AA0__web6(node, owner); break;
        case 6: func_00205D08__web6(node, owner); break;
        default: *(int *)((char *)node + 0x40) = 0; break;
        }
        {
            struct ArenaStack__web6 *arena = (struct ArenaStack__web6 *)D_0029D99C;
            int p = arena->x8;
            arena->x4 = p;
            arena->x8 = *(int *)p;
        }
        break;
    }
    }
}
/* ---- merged from src/unsorted/00204E48.c ---- */
extern "C" {
struct Fade__c38 {
    char pad0[0x5];
    unsigned char type;
    char pad6[0x3A];
    float x40;
    float duration;
    float start;
};

extern int D_0029E160__c38 asm("D_0029E160");
extern int D_0029D994__c38 asm("D_0029D994");

extern float func_00204E48__c38(struct Fade__c38 *fade) asm("func_00204E48");
float func_00204E48__c38(struct Fade__c38 *fade)
{
    float elapsed;
    float t;
    float scale;

    switch (fade->type) {
    case 2:
        elapsed = ((float)D_0029E160__c38 - fade->start) * 16.66667f;
        if (elapsed <= fade->duration) {
            t = elapsed / fade->duration;
            switch (D_0029D994__c38) {
            case 0:
            default:
                scale = t;
                break;
            case 1:
                scale = t * ((t * -2.0f + 3.0f) * t);
                break;
            case 2:
                scale = t * t;
                break;
            case 3:
                scale = t * (2.0f - t);
                break;
            }
            return scale * fade->x40;
        }
        /* fall through */
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    default:
        return fade->x40;
    }
}
}
#ifdef NONMATCHING
/* Non-matching: sched2 loads the TO pose fields before FROM, sched1 puts movz before the tail-mask andi;
   best diff 8/286 words */
extern "C" {
/* A pose blend between two keyframes; channels flagged as angles blend the short way round. */
struct Blend {
    char pad0[5]; unsigned char type; char pad6[2];
    float *values; unsigned char *flags; char pad10[12];
    struct Node *track; char pad20[32];
    float weight, duration, start, x4C;
};
struct PoseHeader { unsigned short x0, count; short x4, angleOffset; };
struct BlendOwner { char pad[32]; struct PoseHeader *header; };
struct Pose { float *values; unsigned char *flags; };
struct Bits64 { unsigned long value; } __attribute__((packed));
struct BlendPool { Node__c2 nodes[128]; unsigned char free[128]; unsigned char count; };
extern struct BlendPool D_004CD480__blend asm("D_004CD480");
extern int D_0029E160__blend asm("D_0029E160");
extern void func_00205D10__blend(Blend *, BlendOwner *, Pose **) asm("func_00205D10");
extern void func_00205F40__blend(Blend *, BlendOwner *, Pose **) asm("func_00205F40");
extern void func_002061F0__blend(Blend *, BlendOwner *) asm("func_002061F0");

/* abs.s kept in place: loads/stores don't move across it (docs/WORKER.md, finding 013) */
static inline float Abs(float x)
{
    float r;
    __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x));
    return r;
}

/* wrap an angle into [-pi, pi] */
#define WRAP_ANGLE(a) { \
    if (!(Abs(a) < 3.1415927f)) { \
        float turns = (a) * 0.15915494f; \
        turns -= (float)(int)turns; \
        if (turns > 0.5f) \
            turns -= 1.0f; \
        else if (turns <= -0.5f) \
            turns += 1.0f; \
        (a) = turns * 6.2831855f; \
    } \
}

extern void func_00204F38__blend(Blend *, BlendOwner *) asm("func_00204F38");
void func_00204F38__blend(Blend *blend, BlendOwner *owner)
{
    Pose *from = 0;
    Pose *to = 0;
    float elapsed = ((float)D_0029E160__blend - blend->start) * 16.66667f;
    float t, ease, weight, a, b, diff;
    struct PoseHeader *header;
    unsigned char *angleBits, *fromFlags, *toFlags, *outFlags;
    float *fromValues, *toValues, *out;
    unsigned long fromBits, toBits, angles;
    unsigned long fromBit, toBit, angleBit;
    int tailBits;
    int count, blocks, remaining, bytes, n;

    if (elapsed < blend->duration) {
        func_00205D10__blend(blend, owner, &from);
        func_00205F40__blend(blend, owner, &to);
        t = elapsed / blend->duration;
        switch (D_0029D994) {
        case 0:
        default:
            ease = t;
            break;
        case 1:
            ease = t * ((t * -2.0f + 3.0f) * t);
            break;
        case 2:
            ease = t * t;
            break;
        case 3:
            ease = t * (2.0f - t);
            break;
        }
        blend->weight = 1.0f - ease;

        header = owner->header;
        if (header->count > 16)
            angleBits = (unsigned char *)header + header->angleOffset;
        else
            angleBits = (unsigned char *)header + 6;
        count = header->count;
        toValues = to->values;
        toFlags = to->flags;
        fromValues = from->values;
        blocks = (count >> 6) + 1;
        weight = blend->weight;
        do {} while (0); /* empty blocks only steer register allocation/scheduling */
        fromFlags = from->flags;
        do {} while (0);
        outFlags = blend->flags;
        out = blend->values;
        /* 64 channels per block; the last block holds count & 63 */
        while (blocks != 0) {
            toBits = ((Bits64 *)toFlags)->value;
            fromBits = ((Bits64 *)fromFlags)->value;
            angles = ((Bits64 *)angleBits)->value;
            do {} while (0);
            int one = 1; /* constant 1; the named temporary only steers scheduling */
            bytes = 8;
            do {} while (0);
            if (blocks <= one)
                bytes = ((count >> 3) & 7) + 1;
            do {} while (0);
            one = 0;
            remaining = blocks - 1;
            tailBits = count & 63;
            do {} while (0);
            angleBits += 8;
            while (bytes != 0) {
                *outFlags++ = *toFlags++ | *fromFlags++;
                bytes--;
            }
            n = blocks > 1 ? 64 : tailBits;
            while (n != 0) {
                fromBit = fromBits & 1;
                toBit = toBits & 1;
                if (fromBit & toBit) {
                    angleBit = angles & 1;
                    a = *fromValues;
                    b = *toValues;
                    if (angleBit) {
                        WRAP_ANGLE(a);
                        WRAP_ANGLE(b);
                        diff = a - b;
                        WRAP_ANGLE(diff);
                    } else {
                        diff = a - b;
                    }
                    *out = b + diff * weight;
                } else if (fromBit) {
                    *out = *fromValues;
                } else if (toBit) {
                    *out = *toValues;
                }
                n--;
                toBits >>= 1;
                fromBits >>= 1;
                angles >>= 1;
                out++;
                toValues++;
                fromValues++;
            }
            blocks = remaining;
        }
    } else {
        int freeCount;

        func_002061F0__blend(blend, owner);
        to = (Pose *)&blend->values;
        func_00205F40__blend(blend, owner, &to);
        if (blend->type == 7) {
            AnimationHandle handle;

            handle.x0 = (AnimationOwner *)owner;
            handle.x8 = 0;
            handle.x4 = (AnimationTrack *)blend->track;
            handle.xC = blend->track->x4->x10;
            func_00203B68(&handle, 1, blend->x4C);
            func_00203D18(&handle);
        }
        /* give the blend node back to the pool */
        freeCount = D_004CD480__blend.count - 1;
        D_0029D998 = 1;
        D_004CD480__blend.count = freeCount;
        D_004CD480__blend.free[freeCount & 255] = (Node__c2 *)blend - D_004CD480__blend.nodes;
    }
}
#undef WRAP_ANGLE
}
#else
INCLUDE_ASM("asm/nonmatchings/text", func_00204F38);
#endif
/* ---- decompiled func_002053B0 ---- */
/* Blend the second channel set (pair) into obj's values by the fade weight:
   64 values per block, srcMask bits select which ones; the mask bytes are OR-ed in. */
extern "C" {
typedef struct Node__c142 { unsigned short x0; unsigned short x2; } Node__c142;
typedef struct Arg__c142 { char pad0[0x20]; Node__c142 *x20; } Arg__c142;
typedef struct Obj__c142 { char pad0[8]; float *x8; unsigned char *xC; char pad10[0x30]; float x40; float x44; float x48; } Obj__c142;
typedef struct Pair__c142 { float *x0; unsigned char *x4; } Pair__c142;
typedef struct PackedU64__c142 { unsigned long x; } __attribute__((packed)) PackedU64__c142;
extern int D_0029E160__c142 asm("D_0029E160");
extern void func_00205D10__c142(Obj__c142 *, Arg__c142 *, void **) asm("func_00205D10");
extern void func_00205F40__c142(Obj__c142 *, Arg__c142 *, void **) asm("func_00205F40");
extern void func_002053B0__c142(Obj__c142 *obj, Arg__c142 *arg) asm("func_002053B0");
void func_002053B0__c142(Obj__c142 *obj, Arg__c142 *arg) {
    void *p0;
    Pair__c142 *pair;
    float elapsed;
    float t;
    float weight;
    float inv;
    unsigned char *dstMask;
    unsigned char *srcMask;
    float *src;
    float *dst;
    int count;
    int nBlocks;
    int nBytes;
    int nBits;
    unsigned long bits;
    unsigned char *d;
    p0 = 0;
    pair = 0;
    elapsed = ((float)D_0029E160__c142 - obj->x48) * 16.66667f;
    if (elapsed < obj->x44) {
        t = elapsed / obj->x44;
        switch (D_0029D994) {
        case 0:
        default:
            weight = t;
            break;
        case 1:
            weight = ((t * -2.0f + 3.0f) * t) * t;
            break;
        case 2:
            weight = t * t;
            break;
        case 3:
            weight = t * (2.0f - t);
            break;
        }
    } else {
        weight = obj->x40;
    }
    if (weight < 1e-10f)
        return;
    func_00205D10__c142(obj, arg, &p0);
    func_00205F40__c142(obj, arg, (void **)&pair);
    if (pair == 0)
        return;
    count = arg->x20->x2;
    inv = 1.0f - weight;
    nBlocks = (count >> 6) + 1;
    srcMask = pair->x4;
    dstMask = obj->xC;
    src = pair->x0;
    dst = obj->x8;
    while (nBlocks != 0) {
        bits = ((PackedU64__c142 *)srcMask)->x;
        /* Empty loop + pointer copy: the loop note is a scheduling barrier that keeps
           the unaligned 64-bit load (ldl/ldr) first in the block, as in retail;
           the copy is coalesced away by the register allocator. */
        do {
        } while (0);
        d = dstMask;
        nBytes = (nBlocks > 1) ? 8 : (((count >> 3) & 7) + 1);
        while (nBytes != 0) {
            *d++ |= *srcMask++;
            nBytes--;
        }
        dstMask = d;
        nBits = (nBlocks > 1) ? 64 : (count & 63);
        while (nBits != 0) {
            nBits--;
            if (bits % 2)
                *dst = *dst * inv + *src * weight;
            bits >>= 1;
            dst++;
            src++;
        }
        nBlocks--;
    }
}
}
/* ---- decompiled func_002055D0 ---- */
extern "C" {
typedef struct Obj__c43 { char pad0[8]; float *x8; unsigned char *xC; char pad10[48]; float x40, x44, x48; } Obj__c43;
typedef struct Count__c43 { unsigned short x0, x2; } Count__c43;
typedef struct Owner__c43 { char pad[32]; Count__c43 *x20; } Owner__c43;
typedef struct Buf__c43 { float *x0; unsigned char *x4; } Buf__c43;
extern int D_0029E160__c43 asm("D_0029E160");
extern int D_0029D994__c43 asm("D_0029D994");
extern void func_00205D10__c43(Obj__c43 *, Owner__c43 *, Buf__c43 **) asm("func_00205D10");
extern void func_00205F40__c43(Obj__c43 *, Owner__c43 *, Buf__c43 **) asm("func_00205F40");
extern void func_002055D0__c43(Obj__c43 *obj, Owner__c43 *owner) asm("func_002055D0");
void func_002055D0__c43(Obj__c43 *obj, Owner__c43 *owner) {
    Buf__c43 *out1 = 0;
    Buf__c43 *out2 = 0;
    float elapsed;
    float t;
    float weight;
    int b1;
    int b2;
    int i;
    elapsed = ((float)D_0029E160__c43 - obj->x48) * 16.66667f;
    if (elapsed < obj->x44) {
        t = elapsed / obj->x44;
        switch (D_0029D994__c43) {
        case 0:
        default:
            weight = t;
            break;
        case 1:
            weight = ((t * -2.0f + 3.0f) * t) * t;
            break;
        case 2:
            weight = t * t;
            break;
        case 3:
            weight = t * (2.0f - t);
            break;
        }
    } else {
        weight = obj->x40;
    }
    if (weight < 1e-10f) return;
    func_00205D10__c43(obj, owner, &out1);
    func_00205F40__c43(obj, owner, &out2);
    i = 0;
    for (i = 0; i < owner->x20->x2; i++) {
            if (out1 != 0) b1 = (out1->x4[(unsigned int)i >> 3] >> ((unsigned int)i & 7)) & 1;
            else b1 = 0;
            if (out2 != 0) b2 = (out2->x4[(unsigned int)i >> 3] >> ((unsigned int)i & 7)) & 1;
            else b2 = 0;
            if (b1 != 0) {
                if (b2 != 0) {
                    obj->x8[i] = out1->x0[i] * weight + out2->x0[i] * (1.0f - weight);
                    obj->xC[(unsigned int)i >> 3] |= 1 << ((unsigned int)i & 7);
                } else {
                    obj->x8[i] = out1->x0[i] * weight;
                    obj->xC[(unsigned int)i >> 3] |= 1 << ((unsigned int)i & 7);
                }
            } else if (b2 != 0) {
                obj->x8[i] = out2->x0[i] * weight;
                obj->xC[(unsigned int)i >> 3] |= 1 << ((unsigned int)i & 7);
            }
        }
}
}
/* ---- merged from src/unsorted/00205850.c ---- */
extern "C" {
typedef struct Node__c39 { unsigned short x0, x2; char pad4[8]; unsigned short xC; char padE[2]; unsigned short x10; unsigned short x12[1]; } Node__c39;
typedef struct Track__c39 { int x0; Node__c39 *x4; char pad8[0x28]; unsigned short *x30; } Track__c39;
typedef struct Obj__c39 { char pad0[8]; float *x8; unsigned char *xC; char pad10[0xC]; Track__c39 *x1C; char pad20[0x20]; float x40, x44, x48; } Obj__c39;
typedef struct Arg__c39 { char pad0[0x20]; Node__c39 *x20; } Arg__c39;
typedef struct Context__c39 { Arg__c39 *x0; Track__c39 *x4; int x8; unsigned short xC; } Context__c39;
extern int D_0029E160__c39 asm("D_0029E160");
/* D_0029D994 defined at file top (sdata order) */
extern float func_00204770__c39(Context__c39 *) asm("func_00204770");
extern float func_00206E48__c39(float, Node__c39 *, unsigned short *) asm("func_00206E48");
extern void func_002087C0__c39(float *) asm("func_002087C0");
extern void func_00205850__c39(Obj__c39 *obj, Arg__c39 *arg) asm("func_00205850");
void func_00205850__c39(Obj__c39 *obj, Arg__c39 *arg) {
    Context__c39 ctx;
    unsigned char *bits;
    Track__c39 *track;
    Node__c39 *node, *previous, *first;
    unsigned short *values, *indices;
    int count;
    unsigned int index, byte;
    int bit;
    float elapsed, t, scale, weight, phase;
    track = obj->x1C;
    if ((track->x0 & 0x3F) == 0x21) {
        elapsed = ((float)D_0029E160__c39 - obj->x48) * 16.66667f;
        first = track->x4;
        if (elapsed < obj->x44) {
            t = elapsed / obj->x44;
            switch (D_0029D994) {
            case 0: default: scale = t; break;
            case 1: scale = t * ((t * -2.0f + 3.0f) * t); break;
            case 2: scale = t * t; break;
            case 3: scale = t * (2.0f - t); break;
            }
            weight = scale * obj->x40;
        } else weight = obj->x40;
        ctx.x8 = 0;
        ctx.x0 = arg;
        ctx.x4 = track;
        ctx.xC = first->x10;
        phase = func_00204770__c39(&ctx);
        if (arg->x20->x2 <= 16) bits = (unsigned char *)arg->x20 + 6;
        else bits = (unsigned char *)arg->x20 + *(short *)((char *)arg->x20 + 6);
        indices = first->x12;
        previous = first;
        values = track->x30;
        count = previous->xC;
        while (count) {
            node = (Node__c39 *)((char *)previous - *indices * 4);
            index = node->x2;
            obj->x8[index] += weight * CallShared00206E48_0(phase, node, values);
            byte = index >> 3;
            bit = index & 7;
            previous = node;
            if ((bits[byte] >> bit) & 1) func_002087C0__c39(&obj->x8[index]);
            obj->xC[byte] |= 1 << bit;
            count--;
            indices++;
            if ((unsigned int)((node->x0 & 7) - 2) >= 2) values++;
        }
    }
}
}
/* ---- func_00205AA0 attempt 1 ---- */
extern "C" {
typedef struct Obj__c42 { char pad0[8]; float *x8; unsigned char *xC; char pad10[48]; float x40, x44, x48; } Obj__c42;
typedef struct Count__c42 { unsigned short x0, x2; } Count__c42;
typedef struct Owner__c42 { char pad[32]; Count__c42 *x20; } Owner__c42;
typedef struct Buf__c42 { float *x0; unsigned char *x4; } Buf__c42;
extern int D_0029E160__c42 asm("D_0029E160");
extern int D_0029D994__c42 asm("D_0029D994");
extern void func_00205D10__c42(Obj__c42 *, Owner__c42 *, Buf__c42 **) asm("func_00205D10");
extern void func_00205F40__c42(Obj__c42 *, Owner__c42 *, Buf__c42 **) asm("func_00205F40");
extern void func_00205AA0__c42(Obj__c42 *obj, Owner__c42 *owner) asm("func_00205AA0");
void func_00205AA0__c42(Obj__c42 *obj, Owner__c42 *owner) {
    Buf__c42 *out1 = 0;
    Buf__c42 *out2 = 0;
    float elapsed;
    float t;
    float weight;
    int b1;
    int b2;
    int i;
    elapsed = ((float)D_0029E160__c42 - obj->x48) * 16.66667f;
    if (elapsed < obj->x44) {
        t = elapsed / obj->x44;
        switch (D_0029D994__c42) {
        case 0:
        default:
            weight = t;
            break;
        case 1:
            weight = ((t * -2.0f + 3.0f) * t) * t;
            break;
        case 2:
            weight = t * t;
            break;
        case 3:
            weight = t * (2.0f - t);
            break;
        }
    } else {
        weight = obj->x40;
    }
    func_00205D10__c42(obj, owner, &out1);
    func_00205F40__c42(obj, owner, &out2);
    i = 0;
    for (i = 0; i < owner->x20->x2; i++) {
            if (out1 != 0) b1 = (out1->x4[(unsigned int)i >> 3] >> ((unsigned int)i & 7)) & 1;
            else b1 = 0;
            if (out2 != 0) b2 = (out2->x4[(unsigned int)i >> 3] >> ((unsigned int)i & 7)) & 1;
            else b2 = 0;
            if (b1 != 0) {
                if (b2 != 0) {
                    obj->x8[i] = out1->x0[i] * weight + out2->x0[i] * (1.0f - weight);
                    obj->xC[(unsigned int)i >> 3] |= 1 << ((unsigned int)i & 7);
                } else {
                    obj->x8[i] -= out1->x0[i] * weight;
                    obj->xC[(unsigned int)i >> 3] |= 1 << ((unsigned int)i & 7);
                }
            } else if (b2 != 0) {
                obj->x8[i] -= out2->x0[i] * weight;
                obj->xC[(unsigned int)i >> 3] |= 1 << ((unsigned int)i & 7);
            }
        }
}
}
/* ---- merged from src/unsorted/00205D08.c ---- */
extern "C" {
extern void func_00205D08__c40(void) asm("func_00205D08");
void func_00205D08__c40(void) {
}
}
void func_00205D10(struct Object *arg0, struct Owner *arg1, void **arg2)
{
 AnimationHandle h;
 struct Node *node;
 int type, size; char *p,*q;
 h.x8=0;
 node=arg0->x18;
 if(node) {
 type=node->x0&63;
 if(type==33) {
  node->x8=allocate(arg1->x20->x2*4);
  node->xC=(char *)allocate((arg1->x20->x2>>3)+1);
  node->x10=arg1->xC;
  h.x0=(AnimationOwner *)arg1; h.x4=(AnimationTrack *)node; h.xC=node->x4->x10;
  func_00203D18(&h);
  node->x14=arg0->x20; node->x18=arg0->x24; node->x1C=arg0->x28;
  func_00204CE0(arg0->x18,arg1);
  node->x1C=arg0->x28;
  arg0->x20=node->x14; arg0->x24=node->x18; arg0->x28=node->x1C;
  *arg2=&node->x8;
 } else if(type==43) {
  node->xC=allocate(arg1->x20->x2*4);
  node->x10=(int)allocate((arg1->x20->x2>>3)+1);
  node->x14=arg1->xC;
  func_00204CE0(arg0->x18,arg1);
  *arg2=&node->xC;
 } else if(type==34) {
  node->x8=allocate(arg1->x20->x2*4);
  node->xC=(char *)allocate((arg1->x20->x2>>3)+1);
  node->x10=arg1->xC;
  func_00204CE0(arg0->x18,arg1);
  arg0->x20=node->x30; arg0->x24=node->x34; arg0->x28=node->x38;
  *arg2=&node->x8;
 }
 }
}
/* func_00205F40: sibling of func_00205D10 with a shared-buffer fast path. */
struct Pair__c41 { long a; int b; } __attribute__((packed,aligned(4)));
struct Obj__c41 {
    char pad[5];
    unsigned char x5;
    char pad6[22];
    struct Node *x1C;
    char pad20[16];
    int x30;
    float x34, x38;
};
void func_00205F40(struct Obj__c41 *arg0, struct Owner *arg1, void **arg2)
{
 AnimationHandle h;
 struct Node *node = arg0->x1C;
 int type;
 h.x8 = 0;
 if (node) {
  type = node->x0 & 63;
  if (type == 33) {
   if (!*arg2) {
    node->x10 = arg1->xC;
    node->x8 = allocate(arg1->x20->x2 * 4);
    node->xC = allocate((arg1->x20->x2 >> 3) + 1);
   } else {
    *(struct Pair__c41 *)&node->x8 = *(struct Pair__c41 *)*arg2;
   }
   h.x0 = (AnimationOwner *)arg1; h.x4 = (AnimationTrack *)node; h.xC = node->x4->x10;
   func_00203D18(&h);
   node->x14 = arg0->x30; node->x18 = arg0->x34;
   if (arg0->x5 == 7) node->x1C = 0; else node->x1C = arg0->x38;
   func_00204CE0(arg0->x1C, arg1);
   node->x1C = arg0->x38;
   arg0->x30 = node->x14; arg0->x34 = node->x18; arg0->x38 = node->x1C;
   *arg2 = &node->x8;
  } else if (type == 43) {
   if (!*arg2) {
    node->xC = allocate(arg1->x20->x2 * 4);
    node->x10 = (int)allocate((arg1->x20->x2 >> 3) + 1);
    node->x14 = arg1->xC;
   } else {
    *(struct Pair__c41 *)&node->xC = *(struct Pair__c41 *)*arg2;
   }
   func_00204CE0(arg0->x1C, arg1);
   *arg2 = &node->xC;
  } else if (type == 34) {
   if (!*arg2) {
    node->x8 = allocate(arg1->x20->x2 * 4);
    node->xC = allocate((arg1->x20->x2 >> 3) + 1);
    node->x10 = arg1->xC;
   } else {
    *(struct Pair__c41 *)&node->x8 = *(struct Pair__c41 *)*arg2;
   }
   func_00204CE0(arg0->x1C, arg1);
   arg0->x30 = node->x30; arg0->x34 = node->x34; arg0->x38 = node->x38;
   *arg2 = &node->x8;
  }
 }
}
