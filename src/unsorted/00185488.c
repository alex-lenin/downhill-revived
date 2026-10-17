/* UNIT 0x00185488 0x00185590 */
/* sceMcIconSys layout (icon.sys) */
typedef struct IconSys {
    char head[0xC0];
    char title[0x44];
    char viewName[0x40];
    char copyName[0x40];
    char delName[0x40];
    char reserve[0x3D0 - 0x1C4];
} IconSys;

typedef struct Obj {
    char pad0[0xC8C];
    char *xC8C;
} Obj;

typedef struct Input {
    int x0;
    char pad4[0x8];
    int xC;
    int x10;
    char pad14[0xA1 - 0x14];
    char xA1[1];
} Input;

extern char D_0028C658[];
extern void func_00254544(char *, char *);
extern char *strncat(char *, const char *, unsigned int);
extern int strcmp(const char *, const char *);
extern int func_002460F8(int, int, char *, int);
extern int func_002466A0(int, int *, int *);
extern int func_00246258(int);

void func_00185488(Obj *obj, Input *in, unsigned char *out)
{
    IconSys icon;
    char path[0x50];
    int cmd;
    int fd;
    int result;

    func_00254544(path, obj->xC8C);
    strncat(path, D_0028C658, 0x4A);
    func_002460F8(in->x10, in->xC, path, 3);
    cmd = 2;
    func_002466A0(0, &cmd, &fd);
    if (fd == -1) {
        *out = 0;
        return;
    }
    func_00246258(fd);
    cmd = 3;
    func_002466A0(0, &cmd, &result);
    if (strcmp(icon.viewName, in->xA1) || strcmp(icon.copyName, in->xA1) || strcmp(icon.delName, in->xA1)) {
        *out = 1;
    }
}
