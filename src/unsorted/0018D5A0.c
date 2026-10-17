/* UNIT 0x0018D5A0 0x0018D610 */
typedef struct State { char *x0; int x4; int x8; } State;
extern void func_001AC578(void *);
extern int func_00238F60(void);
extern void func_00157400(void *,int);
extern void func_0012FE38(void *,void *);
void func_0018D5A0(State *state) {
 char *obj = state->x0;
 func_001AC578(obj + 0x1e40);
 state->x8 -= func_00238F60();
 func_00157400(obj + 0x3fd0,1);
 if (state->x8 <= 0) func_0012FE38(obj,obj + 0x4ed0);
}
