/* UNIT 0x0013CA68 0x0013CAF0 */
#include "derived_menu.h"
#if defined(__mips__)
#define InitializeDerivedMenuView(p) func_0018F210(p)
#else
#define InitializeDerivedMenuView(p) func_0018F210((SharedDerivedMenu *)(p))
#endif
typedef struct Obj {
 char pad0[1272];
 void * x4F8;
 char pad4fc[32];
 int x51C[5][2];
 int x544[5][2];
 char pad56c[8];
 int x574;
 char pad578[100];
 int x5DC;
} Obj;
#if defined(__mips__)
extern void func_0018F210(void *);
#endif
extern char D_002840E8[];
Obj *func_0013CA68(Obj *p) {
 int i,j;
 InitializeDerivedMenuView(p);
 p->x574=0; p->x5DC=0; p->x4F8=D_002840E8;
 for(i=0;i<5;i++) for(j=0;j<2;j++) { p->x51C[i][j]=0; p->x544[i][j]=0; }
 return p;
}
