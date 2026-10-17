/* UNIT 0x0019AF60 0x0019B000 */
typedef struct State { char pad[0x500]; int x500; char pad504[8]; signed char x50C[4][13]; unsigned char x540[4]; } State;
extern struct Global { char pad[0x12]; signed char x12; char pad13; int x14[4]; } D_0043DCC0;
void func_0019AF60(State *state,int index,int value) {
 int i; int old,next;
 D_0043DCC0.x14[D_0043DCC0.x12]=1;
 old=state->x50C[state->x500][index];
 state->x50C[state->x500][index]=value;
 for(i=index+1;i<12;i++) {
  next=state->x50C[state->x500][i];
  state->x50C[state->x500][i]=old;
  old=next;
 }
 state->x540[state->x500]++;
}
