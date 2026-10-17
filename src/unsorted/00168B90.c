/* UNIT 0x00168B90 0x00168CD0 */
struct Res;

struct Obj {
    char pad0[0x418];
    int x418;
    char pad41C[0x460 - 0x41C];
    int x460;
    char pad464[0x4A8 - 0x464];
    int x4A8;
};

struct Player {
    char pad0[0x796C];
    int x796C;
    char pad7970[0x798C - 0x7970];
    int x798C;
};

struct Game {
    char pad0[0x50130];
    struct Player *players[1];
};

struct Config {
    char pad0[0x288];
    int count;
};

extern struct Res *D_0029DE68;
extern struct Game D_002AF2C0;
extern struct Config D_004B25A0;

extern void func_0018E638(struct Obj *obj, int id, struct Res **res);
extern char *func_00158650(int id);
extern void func_0018E408(struct Obj *obj, char *text, int flag);
extern void func_0018E6B8(struct Obj *obj, int id, struct Res **res);
extern void func_0018DB08(struct Obj *obj);

void func_00168B90(struct Obj *obj)
{
    int place;
    int i;
    struct Player *p;

    obj->x418 = 0;
    func_0018E638(obj, 0x47, &D_0029DE68);
    for (place = 0; place < D_004B25A0.count; place++) {
        for (i = 0; i < D_004B25A0.count; i++) {
            p = D_002AF2C0.players[i];
            if (p->x798C == place + 1) {
                func_0018E408(obj, func_00158650(p->x796C), 1);
            }
        }
    }
    func_0018E6B8(obj, 0x10, &D_0029DE68);
    obj->x4A8 = 5;
    obj->x460 = 0x50;
    func_0018DB08(obj);
}
