/* UNIT 0x001E1F50 0x001E1FB8 */
struct Obj {
    char pad0[0x500];
    int x500;
    int x504;
};

extern char D_0043F870[];
extern int D_004C377C[];
extern void func_001A4938(char *, int, int);
extern void func_0018DC20(struct Obj *);

void func_001E1F50(struct Obj *obj)
{
    if (obj->x500 < 190) {
        obj->x500 += 3;
    } else {
        obj->x504++;
    }
    func_001A4938(D_0043F870, obj->x500, D_004C377C[0]);
    func_0018DC20(obj);
}
