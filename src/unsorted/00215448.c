/* UNIT 0x00215448 0x00215A18 .rodata=0x298330:0x2983E0 */
#include "vector.h"
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int Quad __attribute__((mode(TI)));
typedef Vector4 Vec;
typedef Matrix4 Matrix;
typedef struct Node { u32 flags; char rest[0xCC]; } Node;
typedef struct Entry { Node **x0; int x4; Matrix *x8; u16 xc,xe; Node *x10; } Entry;
typedef struct Select { u32 x0,x4; u8 index; char pad[3]; Node *children[1]; } Select;
typedef struct Result { Node *x0; Matrix *x4; u16 x8,xa; } Result;
extern int D_0029E194;
extern Node *D_0029E198;
extern u8 D_0029E19C;
extern Entry D_00514ADC[], D_00514AF0[], D_005156A8[];
#define STATE (D_005156A8)


extern void func_00214EB0(Node *,Node *,Matrix **);
#define U32(p,o) (*(u32 *)((char *)(p)+(o)))
#define U16(p,o) (*(u16 *)((char *)(p)+(o)))
#define U8(p,o) (*(u8 *)((char *)(p)+(o)))
#define PTR(p,o) (*(Node **)((char *)(p)+(o)))
#define ADD(p,o) ((void *)((char *)(p)+(o)))
#define SUBV(dst,a,b) __asm__ volatile("lqc2 $vf11, 0x0(%1)\n\tlqc2 $vf12, 0x0(%2)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, %0" : "=m"(*(Vec *)(dst)) : "r"(a),"r"(b) )
/* Matrix copy follows the Sony libvu0 load/store sequence. */
static inline void CopyMatrix(Matrix *out, Matrix *in) {
 __asm__ volatile("lq $8, 0x0(%4)
	lq $9, 0x10(%4)
	lq $10, 0x20(%4)
	lq $11, 0x30(%4)
	sq $8, %0
	sq $9, %1
	sq $10, %2
	sq $11, %3" : "=m"(out->row[0]),"=m"(out->row[1]),"=m"(out->row[2]),"=m"(out->row[3]) : "r"(in) : "$8","$9","$10","$11","memory");
}
static inline void push(Node **children, u32 count) {
 Entry *e = &D_00514AF0[D_0029E194];
 e->x0=children;
 e->x4=count;
 e->x8=STATE->x8;
 e->x10=STATE->x10;
 e->xe=STATE->xe;
 e->xc=STATE->xc;
 ++D_0029E194;
}
static inline int get_next(Node **node) {
  {
   Entry *state=STATE;
   Entry *e=&D_00514ADC[D_0029E194];
   if (D_0029E194>0) {
    *node=*e->x0;
    state->xe=e->xe;
    state->xc=e->xc;
    state->x8=e->x8;
    state->x10=e->x10;
    if (--e->x4<=0) --D_0029E194;
    else ++e->x0;
   } else D_0029E194=-1;
  }

 return D_0029E194>=0;
}
int func_00215448(Result *out) {
 Vec vec;
 Matrix *matrix;
 Node *node;
 while (get_next(&node)) {
repeat:
  if ((node->flags & 0x3F) != 25) {
   if (node->flags & 0xFFFC0000) STATE->xe=node->flags>>18;
   if (node->flags & 0x3FF80) STATE->xc=(node->flags>>7)&0x7FF;
  }
  switch (node->flags & 0x3F) {
  case 1: {
   Node *old=node;
   if (!(U16(node,10)&2)) break;
   if (!U16(node,8)) break;
   node=PTR(node,0x20);
   if (U16(old,8)>1) push((Node **)ADD(old,0x24),U16(old,8)-1);
   goto repeat;
  }
  case 2: node=PTR(node,0x28); goto repeat;
  case 3: {
   Node *old=node;
   Matrix *prev=STATE->x8;
   STATE->x8=prev+1;
   CopyMatrix(STATE->x8,prev);
   SUBV(ADD(STATE->x8,0x30),ADD(STATE->x8,0x30),ADD(node,0x10));
   if (!U32(node,8)) break;
   node=PTR(node,0x1C);
   if (U32(old,8)>1) push((Node **)ADD(old,0x20),U32(old,8)-1);
   goto repeat;
  }
  case 4: {
   Node *old=node;
   if (!U32(node,8)) break;
   { Matrix *prev=STATE->x8;
   STATE->x8=prev+1;
     func_00221A60(STATE->x8,ADD(node,0x10),prev); }
   SUBV(&vec,ADD(STATE->x8,-0x10),ADD(node,0x40));
   func_00221888(ADD(node,0x10),&vec,ADD(STATE->x8,0x30));
   node=PTR(node,0x50);
   if (U32(old,8)>1) push((Node **)ADD(old,0x54),U32(old,8)-1);
   goto repeat;
  }
  case 6: node=((Select *)node)->children[((Select *)node)->index]; goto repeat;
  case 39: D_0029E198=node;
  case 35: node=PTR(node,0xC); goto repeat;
  case 31: node=PTR(node,0xC+U32(D_0029E198,4)*12); goto repeat;
  case 8: {
   Node *old=node;
   node=PTR(node,4);
   push((Node **)ADD(old,8),1);
   goto repeat;
  }
  case 10: case 18: case 42:
   if (D_0029E19C) break;
   out->x0=node;
   out->x4=STATE->x8;
   out->x8=STATE->xe;
   out->xa=STATE->xc;
   return 1;
  case 11:
   if (!D_0029E19C) break;
   out->x0=node;
   out->x4=STATE->x8;
   out->x8=STATE->xe;
   out->xa=STATE->xc;
   return 1;
  case 17: {
   Node *old=node;
   if (!(*(unsigned long *)ADD(node,0xC0)&0x40000)) break;
   if (!U32(node,4)) break;
   func_00214EB0(node,STATE->x10,&matrix);
   if (*(float *)ADD(node,0xC)==0.0f) break;
   { Matrix *prev=STATE->x8;
   STATE->x8=prev+1;
     func_00221A60(STATE->x8,matrix,prev); }
   SUBV(&vec,ADD(STATE->x8,-0x10),ADD(matrix,0x30));
   func_00221888(matrix,&vec,ADD(STATE->x8,0x30));
   node=PTR(node,0xCC);
   if (U32(old,4)>1) push((Node **)ADD(old,0xD0),U32(old,4)-1);
   goto repeat;
  }
  case 25: {
   Node *old=node;
   if (!U32(node,0x28)) break;
   node=PTR(node,0x2C);
   STATE->x10=old;
   if (U32(old,0x28)>1) push((Node **)ADD(old,0x30),U32(old,0x28)-1);
   goto repeat;
  }
  case 44: node=PTR(node,4); goto repeat;
  }
 }
 out->x0=0;
 return 0;
}
