/* UNIT 0x0019E898 0x0019E930 */
#include "menu.h"
typedef struct Obj {
    char pad0[0x4F8];
    void *x4F8;
    char pad4FC[0x24];
    char x520;
} Obj;

extern char D_0043DD38[];
extern Obj D_0043DD50;
extern Obj D_0043E280;
extern Obj D_0043E8B0;
extern char D_00287D10[];
extern char D_00287C88[];
extern char D_00287C00[];

extern void func_0019C1F0(void *p);

extern Obj *func_00187378(Obj *obj);

void func_0019E898(int enable, int id)
{
    if (id == 0xFFFF && enable != 0) {
        func_0019C1F0(D_0043DD38);
        func_0018D9D0((SharedMenu *)(&D_0043DD50));
        D_0043DD50.x4F8 = D_00287D10;
        D_0043DD50.x520 = 0;
        func_00187378(&D_0043E280);
        D_0043E280.x4F8 = D_00287C88;
        func_00187378(&D_0043E8B0);
        D_0043E8B0.x4F8 = D_00287C00;
    }
}
