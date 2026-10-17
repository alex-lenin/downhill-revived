/* UNIT 0x0012A4D0 0x0012A570 */
#include "rider_geometry.h"

extern float func_00238F40(void);
extern void func_00236558(void *,void *,float);
extern void func_0012B188(void *);
extern void func_00129FC8(void *);
extern int func_00222BD8(int,int);
extern void func_00129860(void *,int);
void func_0012A4D0(char *obj) {
 float vec[4];
 void *pos = obj+0xd0;
 func_00260E08(obj+0x80, pos, obj+0xe0, vec);
 func_00236558(pos,vec,func_00238F40());
 func_0012B188(obj);
 func_00129FC8(obj);
 if (*(int *)(obj+0x174)>=181 && func_00222BD8(0,100)==42) func_00129860(obj,4);
}
