/* UNIT 0x00186320 0x00186448 */
typedef struct Task { char p0[0x8]; char *x8; unsigned char xC; } Task;
typedef struct Input { int x0; int x4; char p8[0x4]; int xC; int x10; } Input;
typedef struct IconSys { char p0[0x104]; char x104[0x40]; char x144[0x40]; char x184[0x240]; } IconSys;

extern void func_00185DE0(Input *, char *);
extern void func_00254544(char *, char *);
extern char *strncat(char *, const char *, unsigned int);
extern int func_002460F8(int, int, char *, int);
extern int func_002466A0(int, int *, int *);
extern int func_002464B8(int, IconSys *, int);
extern int func_00246258(int);
extern void func_00184938(Input *, int);
extern char D_0028C658[];

int func_00186320(Task *task, Input *in)
{
    IconSys icon;
    char path[0x50];
    int type;
    int fd;
    int result;

    if (in->x0 != 0)
        return 1;
    if (task->xC) {
        func_00185DE0(in, task->x8);
        return 1;
    }
    func_00254544(path, task->x8);
    strncat(path, D_0028C658, 0x4A);
    func_002460F8(in->x10, in->xC, path, 3);
    type = 2;
    func_002466A0(0, &type, &fd);
    if (fd == -1) {
        func_00184938(in, in->x4);
        return 1;
    }
    icon.x104[0] = 0;
    icon.x144[0] = 0;
    icon.x184[0] = 0;
    func_002464B8(fd, &icon, 0x3C4);
    type = 6;
    func_002466A0(0, &type, &result);
    func_00246258(fd);
    type = 3;
    func_002466A0(0, &type, &result);
    return 1;
}
