/* UNIT 0x00151CE8 0x00151D90 */
#include "menu.h"
/* Static initialization for this file's global objects (initialize, priority 0xFFFF). */
struct Obj {
    char pad0[0x4F8];
    void *vtbl;
};

extern struct Obj D_0037F870;
extern struct Obj D_0037FD70;
extern char D_00380270[];
extern char D_00380770[];
extern char D_00380D00[];
extern struct Obj D_003817D0;
extern char D_002845E0[];
extern char D_002844D0[];
extern char D_00284558[];


extern void func_00150410(void *obj);
extern void func_00150890(void *obj);
extern void func_00150E80(void *obj);

void func_00151CE8(int initialize, int priority) {
    if (priority == 0xFFFF && initialize) {
        func_0018D9D0((SharedMenu *)(&D_0037F870));
        D_0037F870.vtbl = D_002845E0;
        func_0018D9D0((SharedMenu *)(&D_0037FD70));
        D_0037FD70.vtbl = D_002844D0;
        func_00150410(D_00380270);
        func_00150890(D_00380770);
        func_00150E80(D_00380D00);
        func_0018D9D0((SharedMenu *)(&D_003817D0));
        D_003817D0.vtbl = D_00284558;
    }
}
