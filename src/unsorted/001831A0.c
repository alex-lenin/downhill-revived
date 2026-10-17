/* UNIT 0x001831A0 0x00183218 .rodata=0x28C5A0:0x28C5C0 */
struct Loader {
    char pad0[0x8];
    int x8;
    char padC[0xA0 - 0xC];
    int xA0;
};

extern char D_004B25A0[];
extern void func_002214B0(void);
extern void func_0025AF30(int);
extern void func_0025A3E0(int, int);
extern int func_002214E0(void);
extern void func_00254D08(const char *);
extern void func_001B8CD8(void *, int);
extern void func_00238A68(int);
extern void func_00209060(int);

void func_001831A0(struct Loader *loader)
{
    func_002214B0();
    func_0025AF30(2);
    func_0025A3E0(2, loader->xA0);
    if (func_002214E0() == -1) {
        func_00254D08("Could'nt delete loadbar thread\n");
    }
    func_001B8CD8(D_004B25A0, 1);
    func_00238A68(0x3C);
    func_00209060(loader->x8);
}
