/* UNIT 0x0014EC78 0x0014ECE0 */
struct Obj { int x0, x4; };
struct State { char pad[0x796C]; int x796C; };
struct Global { char pad[0x50130]; struct State *x50130; };
extern struct Global D_002AF2C0;
extern char D_004225A0[];
extern int func_00194C80(void *, int);
int func_0014EC78(struct Obj *obj) {
    return obj->x0 == func_00194C80(D_004225A0 + ((D_002AF2C0.x50130->x796C >> 5) - 1) * 0x694, obj->x4);
}
