/* UNIT 0x001B5788 0x001B57F8 */
#include "render_wait.h"
extern char D_004B1F40[], D_004B25A0[];
extern void func_0025A960(int), func_002433A8(int,int), func_001B8CD8(void *,int), func_00218A30(void), func_0023C8D8(int,int);
void func_001B5788(unsigned char *p) {
 if(p[3]) {
 WaitForRenderLegacy(D_004B1F40); func_0025A960(0); func_002433A8(0,0); func_001B8CD8(D_004B25A0,1); func_002433A8(0,0); func_00218A30(); func_0023C8D8(5,0);
 }
}
