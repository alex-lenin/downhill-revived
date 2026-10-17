/* UNIT 0x002010A0 0x002010F0 -sce29 */
typedef struct ClassInfo {
    char *name;
    void *vtbl;
    struct ClassInfo *parent;
    int pad;
} ClassInfo;

extern ClassInfo D_00773058;
extern char D_00297770[];
extern ClassInfo D_0029E540;
extern ClassInfo *func_002003C0(void);
extern void func_001FFDC8(ClassInfo *info, char *name, ClassInfo *parent);

ClassInfo *func_002010A0(void)
{
    ClassInfo *info = &D_00773058;

    if (info->name == 0) {
        func_002003C0();
        func_001FFDC8(info, D_00297770, &D_0029E540);
    }
    return info;
}
