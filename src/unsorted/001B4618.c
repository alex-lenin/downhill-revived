/* UNIT 0x001B4618 0x001B4688 */
#include "render_wait.h"
extern volatile int D_0029E1C8,D_0029E1CC,D_0029E1E4;
extern void func_002433A8(int,int);
void func_001B4618(void) {
    if(D_0029E1C8 | D_0029E1CC | D_0029E1E4) {
        do {} while(D_0029E1C8 | D_0029E1CC | D_0029E1E4);
    }
    func_002433A8(0,0);
}
