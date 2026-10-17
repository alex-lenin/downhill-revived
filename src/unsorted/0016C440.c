/* UNIT 0x0016C440 0x0016C490 */
struct State { char pad[8]; int x8; char gap[0x5348]; int x5354; };
extern struct State D_00398C40;
extern char D_0039E840[];
extern void func_001DDCF0(void *);
extern void func_001BBEF8(void *);
extern void func_001ABBE8(void *, int);
static __inline__ void cleanup(struct State *state) {
    func_001DDCF0(&state->x8);
    func_001BBEF8(&state->x5354);
}
void func_0016C440(int enabled, int value) {
    if (value == 0xFFFF && enabled != 0) {
        cleanup(&D_00398C40);
        func_001ABBE8(D_0039E840, 8);
    }
}
