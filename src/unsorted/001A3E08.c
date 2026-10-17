/* UNIT 0x001A3E08 0x001A3F90 */
typedef struct { char pad[0x796C]; int x796C; } Rider;
typedef struct { Rider *x0; int x4; float x8; int xC,x10; } Entry;
typedef struct { Entry entries[17]; int x154; int x158; } Obj;
extern int D_003F52F0[];
extern char D_0029D2F8[];
extern void func_0023D650(int, int *, int *);
extern void func_0020FB80(int,int,int,int,int);
extern void func_0020F518(int,int,int,int,int);
extern char *func_00158668(int);
extern void func_0020F9A8(int,int,int,int,char *);
extern int func_00254D68(char *,const char *,...);

int func_001A3E08(Obj *obj, int index, int alpha, int arg3, int arg4)
{
    char buffer[32];
    int width, height;
    int font = D_003F52F0[0];
    int y;
    int step = 10;
    func_0023D650(font, &width, &height);
    width /= 2;
    func_0020FB80(0, 0x120, 0xC0, -0x6B, -0x20);
    y = step * index + 0x30;
    func_0020F518(0, 0x58, 0x98, 0x49, alpha);
    func_0020F9A8(0, font, 0x46, y, func_00158668((obj->entries[index].x0->x796C >> 5) - 1));
    func_00254D68(buffer, D_0029D2F8, obj->entries[index].xC);
    func_0020F9A8(0, font, 0xAA, y, buffer);
    func_00254D68(buffer, D_0029D2F8, arg3);
    func_0020F9A8(0, font, 0x122, y, buffer);
    func_00254D68(buffer, D_0029D2F8, arg4);
    func_0020F9A8(0, font, 0x172, y, buffer);
    return 1;
}