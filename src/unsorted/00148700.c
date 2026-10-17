/* UNIT 0x00148700 0x00148740 */
typedef unsigned int u128 __attribute__((mode(TI)));
struct Obj { char pad[0x2B0]; int x2B0; char *x2B4; };
extern void func_00231A08(void *, void *);
void func_00148700(struct Obj *obj) {
 if (obj->x2B0) { char *p = obj->x2B4; if (p) { u128 zero = 0; func_00231A08(p + 0x40C0, &zero); } }
}
