/* UNIT 0x001E2D78 0x001E2E28 */
typedef struct State {
 char pad0[0x121A7];
 unsigned char names[4][12];
} State;

typedef struct Obj {
 char pad0[0x520];
 unsigned char names[4][13];
 unsigned char count[4]; /* characters that are not '!' (unused slots) */
} Obj;

extern State D_004225A0;

void func_001E2D78(Obj *obj) {
 State *state = &D_004225A0;
 unsigned char *src;
 int i, j;
 for (i = 0; i < 4; i++) {
  /* integer add keeps the retail operand order (offset + base) */
  src = (unsigned char *)(i * 12 + (int)state->names);
  obj->count[i] = 0;
  for (j = 0; j < 12; j++) {
   obj->names[i][j] = src[j];
   if (src[j] != '!') {
    obj->count[i]++;
   }
  }
  obj->names[i][12] = '!';
 }
}
