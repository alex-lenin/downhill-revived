/* UNIT 0x00191640 0x00191748 */
typedef struct {
    char pad0[0x8];
    float x8;
} Result;

typedef struct {
    int x0;
    float x4;
    int x8;
} Fade;

typedef struct {
    char pad0[0x90];
    Fade x90;
    char pad9C[0xA0 - 0x9C];
} Slot;

typedef struct {
    int x0;
    char pad4[0x10 - 0x4];
    Slot slots[3];
    unsigned int **x1F0[3];
    char pad1FC[0x204 - 0x1FC];
    float x204;
    int x208[3];
    int x214;
    char pad218[0x220 - 0x218];
    int x220;
} Obj;

extern float D_0029D070[];
extern char D_003041B0[];
extern void *func_00122258(void *obj, unsigned int id);
extern void *func_00191790(Obj *obj, void *id);
extern void func_00191A28(Slot *slot, Obj *obj, void *a, int b, Result *res);

void func_00191640(Obj *obj)
{
    Fade *fade;
    Result *res;
    int i;

    obj->x204 = D_0029D070[0];
    obj->x220 = -1;
    for (i = 0; i < obj->x214; i++) {
        res = 0;
        if (obj->x1F0[i] != 0) {
            res = func_00122258(D_003041B0, **obj->x1F0[i] >> 18);
            res = func_00191790(obj, res);
        }
        if (res != 0) {
            obj->x204 = res->x8;
            func_00191A28(&obj->slots[i], obj, obj->x1F0[i], obj->x208[i], res);
            fade = &obj->slots[i].x90;
            fade->x0 = 0;
            fade->x4 = 12.5f;
            fade->x8 = 0;
        }
    }
    obj->x0 = 1;
}
