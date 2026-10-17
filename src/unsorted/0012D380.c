/* UNIT 0x0012D380 0x0012D3C8 */
typedef struct VTable { char pad[0x38]; short delta; short pad3A; void (*call)(void *); } VTable;
typedef struct Obj { int x0; VTable *vtable; } Obj;
typedef struct State { char pad[0x4ECC]; Obj *x4ECC; } State;
extern void func_0015BFC8(void *);
extern void func_00154058(void *);
void func_0012D380(State *state) {
    Obj *obj;
    VTable *table;
    obj = state->x4ECC;
    table = obj->vtable;
    table->call((char *)obj + table->delta);
    func_0015BFC8((char *)state + 0x63DC);
    func_00154058((char *)state + 0x5F40);
}
