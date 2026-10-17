/* UNIT 0x00186610 0x001866F8 */
#include "scalar_api.h"
typedef struct Name { char s[0x20]; } Name;
typedef struct Scalar00186610Obj { char p0[0x8]; Name names[100]; int count; char *xC8C; } Obj;
typedef struct Slot { int x0; char p4[0x10]; } Slot;
typedef struct Scalar00186610Input { char p0[0xC]; int xC; int x10; char p14[0xBC]; Slot slots[1]; } Input;
typedef struct DirEntry { char p0[0x20]; char name[0x20]; } DirEntry;

extern int func_00261F60(Input *);
extern int func_00254D68(char *, char *, ...);
extern int func_00246960(int, int, char *, int, int, DirEntry *);
extern int func_002466A0(int, int *, int *);
extern void func_00254544(char *, char *);
extern char D_0029CD60[];
extern DirEntry D_003CDD40[];

void func_00186610(Obj *o, Input *in)
{
    char path[0x30];
    int type;
    int i;

    o->count = 0;
    if (in->slots[func_00261F60(in)].x0 == 0)
        return;
    func_00254D68(path, D_0029CD60, o->xC8C);
    func_00246960(in->x10, in->xC, path, 0, 100, D_003CDD40);
    type = 0xD;
    func_002466A0(0, &type, &o->count);
    for (i = 0; i < o->count; i++) {
        func_00254544(o->names[i].s, D_003CDD40[i].name);
    }
}
