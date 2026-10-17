/* UNIT 0x00150468 0x001507E8 */
typedef struct Rider {
    char pad0[0x796C];
    int x796C;
} Rider;

typedef struct Game {
    char pad0[0x50130];
    Rider *x50130;
} Game;

typedef struct Menu {
    char pad0[0xE8];
    int xE8;
} Menu;

extern int D_003F52F0[];
extern Menu *D_0029DE68;
extern Game D_002AF2C0;
extern char D_004225A0[];
extern char D_0028A010[], D_0028A020[], D_0028A038[], D_0028A048[];
extern char D_0028A058[], D_0028A068[], D_0028A078[], D_0028A088[];
extern char D_0029C5F8[];

extern void func_0023D650(int, int *, int *);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020F648(int, int, int);
extern void func_0018D930(void *, int, int, int);
extern void func_0020FB80(int, int, int, int, int);
extern void func_00194D28(char *, unsigned short *, unsigned short *, int *, int *);
extern void func_00194D58(char *, unsigned char *, unsigned char *, unsigned char *, unsigned char *);
extern void func_0020F9A8(int, int, int, int, char *);
extern void func_00254D68(char *, char *, int);
extern void func_0018DC20(void *);

void func_00150468(void *arg0)
{
    char buf[0x30];
    unsigned short v30, v32;
    unsigned char b34, b35, b36, b37;
    int a, b;
    int v40, v44;
    char *p;
    int font;

    font = D_003F52F0[0];
    func_0023D650(font, &a, &b);
    a = a / 2;
    func_0020F518(0, 0xC8, 0xC8, 0, 0x80);
    func_0020F648(0, 0x24, 0x18);
    func_0018D930(arg0, a, 6, D_0029DE68->xE8);
    func_0020FB80(0, 0x120, 0xE0, -0x5B, -0x20);
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0x96);
    p = D_004225A0 + ((D_002AF2C0.x50130->x796C >> 5) - 1) * 0x694;
    func_00194D28(p, &v30, &v32, &v40, &v44);
    func_00194D58(p, &b34, &b35, &b36, &b37);
    func_0020F9A8(0, font, 0x64, 0x28, D_0028A010);
    func_0020F9A8(0, font, 0x64, 0x34, D_0028A020);
    func_0020F9A8(0, font, 0x64, 0x40, D_0028A038);
    func_0020F9A8(0, font, 0x64, 0x4C, D_0028A048);
    func_0020F9A8(0, font, 0x64, 0x58, D_0028A058);
    func_0020F9A8(0, font, 0x64, 0x64, D_0028A068);
    func_0020F9A8(0, font, 0x64, 0x70, D_0028A078);
    func_0020F9A8(0, font, 0x64, 0x7C, D_0028A088);
    func_0020F518(0, 0x58, 0x98, 0x49, 0x96);
    func_00254D68(buf, D_0029C5F8, v30);
    func_0020F9A8(0, font, 0x140, 0x28, buf);
    func_00254D68(buf, D_0029C5F8, v40);
    func_0020F9A8(0, font, 0x140, 0x34, buf);
    func_00254D68(buf, D_0029C5F8, v44);
    func_0020F9A8(0, font, 0x140, 0x40, buf);
    func_00254D68(buf, D_0029C5F8, v32);
    func_0020F9A8(0, font, 0x140, 0x4C, buf);
    func_00254D68(buf, D_0029C5F8, b34);
    func_0020F9A8(0, font, 0x140, 0x58, buf);
    func_00254D68(buf, D_0029C5F8, b35);
    func_0020F9A8(0, font, 0x140, 0x64, buf);
    func_00254D68(buf, D_0029C5F8, b36);
    func_0020F9A8(0, font, 0x140, 0x70, buf);
    func_00254D68(buf, D_0029C5F8, b37);
    func_0020F9A8(0, font, 0x140, 0x7C, buf);
    func_0018DC20(arg0);
}
