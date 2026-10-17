/* UNIT 0x0012E160 0x0012E208 */
typedef struct Model {
    char pad0[0x10];
    char x10[1];
} Model;

typedef struct Source {
    char pad0[0x74];
    int x74;
    int x78;
    char pad7C[0x278 - 0x7C];
    float x278;
} Source;

typedef struct Obj {
    char pad0[0x78D4];
    Source *x78D4;
    char pad78D8[0x792C - 0x78D8];
    Model *x792C;
    char pad7930[0x7954 - 0x7930];
    int x7954;
    int x7958;
    float x795C;
} Obj;

extern char D_0029EA20[];
extern char D_002AF2C0[];

extern int func_00119960(void *table, void *name, float *out);
extern int func_00117960(void *table, Model *model, int id);
extern void func_0011C048(void *list, Obj *obj);

void func_0012E160(Obj *obj)
{
    Source *src = obj->x78D4;
    int id;

    if (src != 0) {
        obj->x7954 = src->x74;
        obj->x7958 = src->x78;
        obj->x795C = src->x278;
        return;
    }
    id = func_00119960(D_0029EA20, obj->x792C->x10, &obj->x795C);
    if (id != 0 && id != obj->x7954) {
        obj->x7954 = id;
        obj->x7958 = func_00117960(D_0029EA20, obj->x792C, id);
        func_0011C048(D_002AF2C0, obj);
    }
}
