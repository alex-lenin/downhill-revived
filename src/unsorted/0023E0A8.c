/* UNIT 0x0023E0A8 0x0023E0F0 */
typedef struct Obj { char *x0; int x4; int x8; } Obj;
extern int func_0023E018(Obj *);
char *func_0023E0A8(Obj *p) {
    return func_0023E018(p) != 0 ? 0 : p->x0 + p->x8 * 0x118000;
}
