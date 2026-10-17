/* UNIT 0x001825B0 0x00182650 */
#include "animation.h"
typedef AnimationHandle Obj;
extern void func_0011C6F0(void *);
extern void func_001DFFF0(void);
extern char D_002FF590[];
void func_001825B0(void) {
    Obj obj;
    int i = 0;
    func_0011C6F0(D_002FF590);
    func_001DFFF0();
    obj.x8 = 0;
    do {
        func_00203590(&obj, 0x32C8, i, 0);
        func_002038E0(&obj, 1);
        func_00203B68(&obj, 1, (float)i * 0.1666f);
        i++;
    } while (i < 6);
}
