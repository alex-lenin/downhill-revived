/* UNIT 0x00185DE0 0x00185FF8 */
typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct Block4 { u128 q[4]; } Block4;
typedef struct Block3 { u128 q[3]; } Block3;

/* PS2 memory card icon.sys header (0x3C4 bytes) */
typedef struct Icon {
    char magic[5];
    char x5;
    short x6;
    int x8;
    int xC;
    Block4 bg;
    Block3 dir;
    Block3 col;
    u128 ambient;
    char title[68];
    char view[64];
    char copy[64];
    char del[64];
    char reserved[512];
} Icon;

extern struct Magic { int x0; unsigned char x4; } D_0029CD70[];
extern Block4 D_0026F280;
extern Block3 D_0026F2C0;
extern Block3 D_0026F2F0;
extern u128 D_0026F320;
extern char D_0028C658[];

extern char *func_00254544(void *, const void *);
extern void strncat(char *, const char *, int);
extern void func_00232EE0(char *, char *);
extern void *func_00254204(void *, int, int);
extern void func_002460F8(int, int, char *, int);
extern int func_002466A0(int, int *, int *);
extern void func_002464B8(int, void *, int);
extern int func_00246258(int);
extern void func_00184938(void *, int);

void func_00185DE0(char *obj, char *name)
{
    Icon icon;
    char path[80];
    char file[64];
    register char *resource = obj + 0xA1;
    int status;
    int fd = -1;
    int result = -1;

    func_00254544(path, name);
    strncat(path, D_0028C658, 74);
    func_00232EE0(obj + 0x81, file);
    func_00254204(&icon, 0, 0x3C4);
    *(int *)icon.magic = D_0029CD70[0].x0;
    icon.magic[4] = D_0029CD70[0].x4;
    func_00254544(icon.title, file);
    icon.x6 = 18;
    icon.xC = 128;
    icon.bg = D_0026F280;
    icon.dir = D_0026F2C0;
    icon.col = D_0026F2F0;
    icon.ambient = D_0026F320;
    func_00254544(icon.view, resource);
    func_00254544(icon.copy, resource);
    func_00254544(icon.del, resource);
    func_002460F8(*(int *)(obj + 0x10), *(int *)(obj + 0xC), path, 0x203);
    status = 2;
    func_002466A0(0, &status, &fd);
    if (fd < 0 || (func_002464B8(fd, &icon, 0x3C4), status = 6,
                   func_002466A0(0, &status, &result), result < 0)) {
        func_00184938(obj, *(int *)(obj + 4));
    } else {
        func_00246258(fd);
        status = 3;
        func_002466A0(0, &status, &result);
        if (result < 0)
            func_00184938(obj, *(int *)(obj + 4));
    }
}
