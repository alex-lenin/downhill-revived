/* UNIT 0x00211DF0 0x00211EC8 */
#include "spatial.h"
typedef struct { int x0; short width,height; char pad8[0x14]; SpatialLink **x1C; unsigned int *x20; int x24; unsigned int cells[1]; } Grid;
typedef SpatialCellIterator Obj;
extern Grid *D_0029DA50;

void func_00211DF0(Obj *obj,int x,int y) {
 int width,index,reverse; unsigned int *start,*end; Grid *cell;
 if(x<0 || x>=(width=D_0029DA50->width) || y<0 || y>=D_0029DA50->height) {
  obj->end=0;obj->cur=0;obj->node=0;obj->entry=0;return;
 }
 index=y*width+x;
 cell=(Grid *)((char *)D_0029DA50+index*4);
 reverse=((int)obj->filter.x4.flags>>18)&1;
 start=D_0029DA50->x20+(cell->cells[0]>>10);
 obj->cur=start;
 end=start+(cell->cells[0]&0x3ff);
 obj->end=end;
 obj->node=D_0029DA50->x1C[index];
 obj->entry=0;
 if(reverse) {obj->cur=end-1;obj->end=start-1;}
 func_00211EC8(obj);
}
