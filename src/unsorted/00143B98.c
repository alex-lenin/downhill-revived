/* UNIT 0x00143B98 0x00143BF8 */
struct Obj { char pad[0x68]; float x68, x6C; };
void func_00143B98(struct Obj *p, int type) {
 if (type == 0x4E2) { p->x68 = 100.0f; p->x6C = 300.0f; }
 else if (type == 0x4DD) { p->x68 = 400.0f; p->x6C = 400.0f; }
 else { p->x68 = 75.0f; p->x6C = 150.0f; }
}
