/* UNIT 0x00185CC8 0x00185D38 */
typedef struct State { int x0; int x4; int x8; int xC; int x10; } State;
extern int func_00246AB0(int,int);
extern int func_002466A0(int,int *,int *);
extern void func_00184938(State *,int);
int func_00185CC8(void *unused, State *state) {
 int value, result;
 if (state->x0) return 1;
 func_00246AB0(state->x10,state->xC);
 value = 0x10;
 func_002466A0(0,&value,&result);
 if (result >= 0) return 1;
 func_00184938(state,state->x4);
 return 1;
}
