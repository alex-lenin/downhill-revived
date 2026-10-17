/* UNIT 0x0018E9D8 0x0018EA20 */
typedef struct Obj Obj;
typedef struct Table { char pad0[8]; short x8; char padA[2]; void (*xC)(void *); } Table;
struct Obj { char pad0[0x4F8]; Table *x4F8; };
extern Obj *func_0018EAC8(int *, int);
void func_0018E9D8(int *arg0) {
    Obj *obj;
    Table *table;
    *arg0 = 0;
    obj = func_0018EAC8(arg0, 0);
    if (obj) {
        table = obj->x4F8;
        table->xC((char *)obj + table->x8);
    }
}

