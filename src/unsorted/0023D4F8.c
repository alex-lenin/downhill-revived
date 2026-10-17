/* UNIT 0x0023D4F8 0x0023D598 */
#include "scalar_api.h"
extern int D_0029E41C, D_0029E420, D_0029E424, D_0029E428;
extern char D_0075BC70[];
void func_0023D4F8(long r, long g, long b, long a) {
 char *p = D_0075BC70;
 long color = r | (g << 8) | (b << 16) | (a << 24) | ((long)0xFE00 << 46);
 do {
  *(long *)p = color;
  *(long *)(p + 0x170) = color;
  p += 0x360;
 } while ((int)p < (int)(D_0075BC70 + 0x1440));
 D_0029E41C = (int)r;
 D_0029E420 = (int)g;
 D_0029E424 = (int)b;
 D_0029E428 = (int)a;
}
