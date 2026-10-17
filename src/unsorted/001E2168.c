/* UNIT 0x001E2168 0x001E21D0 */
struct Obj {
    char pad0[0x460];
    int x460;
    int x464;
    char pad468[0x4A8 - 0x468];
    int x4A8;
};

extern char D_0029DE68[];
extern char D_003F52D0[];
extern void func_0018E638(struct Obj *, int, char *);
extern void func_001B5AF0(char *, int, int);
extern void func_0018DB08(struct Obj *);

void func_001E2168(struct Obj *obj)
{
    obj->x464 = 100;
    obj->x460 = 70;
    func_0018E638(obj, 0x77, D_0029DE68);
    func_001B5AF0(D_003F52D0, 1, 1);
    obj->x4A8 = 5;
    func_0018DB08(obj);
}
