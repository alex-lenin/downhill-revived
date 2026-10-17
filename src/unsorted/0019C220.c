/* UNIT 0x0019C220 0x0019C2F8 */
#include "render_wait.h"
extern char D_004B1F40[];
 extern void sceGsSetDefStoreImage(void*,short,short,int,short,short,short,short);
extern void func_0025A960(int); extern void func_00244228(void*,void*); extern void func_002433A8(int,int);
void func_0019C220(int unused,int base,void *dest,unsigned short x,unsigned short y,int width,int height) { char image[0x70] __attribute__((aligned(16))); WaitForRenderLegacy(D_004B1F40); sceGsSetDefStoreImage(image,(short)base,width/64,0,(short)x,(short)y,(short)width,(short)height); func_0025A960(0); func_00244228(image,dest);func_002433A8(0,0); }
