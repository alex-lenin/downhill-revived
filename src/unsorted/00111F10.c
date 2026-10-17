/* UNIT 0x00111F10 0x00111F98 */
typedef struct Game {
    char pad0[0x7958];
    int x7958;
} Game;

typedef struct Obj {
    char pad0[0x8];
    Game *x8;
    char padC[0x64];
    int x70;
    char pad74[0x4];
    int x78;
} Obj;

extern char D_0029EA20[];

extern void *func_001178A0(void *list, int a, int b);
extern int func_00119810(void *list, void *item);
extern void func_0010B2B8(Obj *obj, void *item);

int func_00111F10(Obj *obj)
{
    void *item;
    int found = 0;

    item = func_001178A0(D_0029EA20, obj->x78, obj->x8->x7958);
    if (item != 0) {
        obj->x70 = func_00119810(D_0029EA20, item);
        found = 1;
        func_0010B2B8(obj, item);
    }
    return found;
}
