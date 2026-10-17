/* UNIT 0x00208210 0x00208240 */
struct S { char pad[0x20]; int x20; };
extern int func_002028C8(void);
void func_00208210(struct S *s) { if (func_002028C8()) s->x20 = 7; }
