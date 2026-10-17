/* UNIT 0x00169DE8 0x00169E90 */
#include "menu.h"
/* Static initialization for this file's global objects (initialize, priority 0xFFFF). */
struct Obj {
    char pad0[0x4F8];
    void *vtbl;
};

extern struct Obj D_00397180;
extern struct Obj D_00397690;
extern struct Obj D_00397BA0;
extern struct Obj D_003980B0;
extern char D_002859E0[];
extern char D_00285958[];
extern char D_002858D0[];
extern char D_00285848[];

/* base constructor, returns the object */


void func_00169DE8(int initialize, int priority) {
    if (priority == 0xFFFF && initialize) {
        func_0018D9D0((SharedMenu *)(&D_00397180));
        D_00397180.vtbl = D_002859E0;
        func_0018D9D0((SharedMenu *)(&D_00397690));
        D_00397690.vtbl = D_00285958;
        func_0018D9D0((SharedMenu *)(&D_00397BA0));
        D_00397BA0.vtbl = D_002858D0;
        func_0018D9D0((SharedMenu *)(&D_003980B0));
        D_003980B0.vtbl = D_00285848;
    }
}
