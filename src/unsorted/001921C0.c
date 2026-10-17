/* UNIT 0x001921C0 0x00192268 */
typedef struct Position { char pad[0x18]; float x18; char pad1[0x8c]; float xA8; } Position;
typedef struct Rider { char pad[0x792c]; Position *x792C; } Rider;
typedef struct State { char pad[8]; Position *x8; char pad1[8]; Rider *x14; } State;
extern struct Global { char pad[0x501a0]; Rider *x501A0; } D_002AF2C0;
extern void func_00191BC0(State *, int);
extern float func_00222610(void *, void *);
extern void func_00221F50(void *, void *);
void func_001921C0(State *state) {
 Rider *rider = D_002AF2C0.x501A0;
 if (rider && rider->x792C->x18 - state->x8->x18 < 300.0f) {
  Position *p;
  func_00191BC0(state, 3);
  state->x14 = rider;
  state->x8->xA8 = func_00222610((char *)state->x8+0x10,(char *)rider->x792C+0x10);
  p=state->x8;
  func_00221F50((char *)p+0x40,(char *)p+0xa0);
 }
}
