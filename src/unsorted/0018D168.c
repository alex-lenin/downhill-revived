/* UNIT 0x0018D168 0x0018D2D0 .sbss=0x29DE60:0x29DE68 */
#include "menu.h"
/* Static constructors of this file's global objects (C++ static init, priority 0xFFFF). */

typedef struct Sub {
    char pad0[0x10];
    void *vptr;
    int x14;
    char x18;
} Sub;

typedef struct Menu {
    char pad0[0x4F8];
    void *vptr;
    char pad4FC[0x534 - 0x4FC];
    Sub x534;
    Sub x550;
} Menu;

typedef struct Handle {
    void *vptr;
    int x4;
} Handle;

typedef struct Profile {
    char pad0[0x2040];
} Profile;

typedef struct Ref {
    void *vptr;
    int x4;
} Ref;

extern Menu D_003EBAC0;
extern Menu D_003EBFD0;
extern Handle D_003EC540[10][4];
extern Profile D_003EC680[4];
Ref D_0029DE60 __attribute__((section(".sbss"))) = { 0 };

extern char D_00286BC0[];
extern char D_002870E8[];
extern char D_00287170[];
extern char D_00286DF8[];
extern char D_0026F490[];
extern char D_00286C48[];


extern void func_0018D6F0(Sub *sub);
extern void func_0018EBC0(Menu *menu);
extern void func_00262270(Handle *handle);
extern void func_002620E0(Profile *profile);

static inline void Sub_Init(Sub *sub)
{
    sub->vptr = D_00287170;
    sub->x14 = 0;
    sub->x18 = 0;
    func_0018D6F0(sub);
}

void func_0018D168(int initialize, int priority)
{
    Handle *row;
    Handle *handle;
    Profile *profile;
    int i;
    int j;
    Handle *next;
    int left;
    int k;

    if (priority == 0xFFFF && initialize) {
        func_0018D9D0((SharedMenu *)(&D_003EBAC0));
        D_003EBAC0.vptr = D_00286BC0;
        func_0018D9D0((SharedMenu *)(&D_003EBFD0));
        D_003EBFD0.vptr = D_002870E8;
        Sub_Init(&D_003EBFD0.x534);
        Sub_Init(&D_003EBFD0.x550);
        func_0018EBC0(&D_003EBFD0);
        D_003EBFD0.vptr = D_00286DF8;
        D_0029DE60.vptr = D_0026F490;
        row = D_003EC540[0];
        i = 9;
        do {
            handle = row;
            next = row + 4;
            left = i - 1;
            j = 3;
            do {
                handle->vptr = D_00286C48;
                func_00262270(handle);
                handle++;
            } while (--j != -1);
            i = left;
            row = next;
        } while (i != -1);
        k = 3;
        profile = D_003EC680;
        do {
            func_002620E0(profile);
            profile++;
        } while (--k != -1);
    }
}
