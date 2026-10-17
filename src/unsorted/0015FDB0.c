/* UNIT 0x0015FDB0 0x0015FE98 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct { char pad[0x1c]; signed int type:16; } Slot;
typedef struct { char pad[0x45c]; int x45c; } Obj;
typedef struct { char pad[4]; char x4; } Child;
typedef struct { char pad[0x12d]; char x12d; char gap[6]; Child *x134; } State;
typedef struct { char pad[0x35c]; char x35c; char gap[0x193]; char x4f0; } Menu;
typedef struct { char pad[0x5be4]; int x5be4, x5be8; } Globals;
extern State D_003F52D0[];
extern Menu D_004B25A0;
extern Globals D_00398C40;
extern char D_0037EA60[];
extern int func_00190D60(void *, int, int);
extern void func_001C57A8(void *, u128);
extern void func_00148560(void *, int);
void func_0015FDB0(Obj *obj, int index) {
 State *state;
 if (!index || (state = D_003F52D0, func_00190D60((char *)state + 0x64, 8, obj->x45c))) {
  D_004B25A0.x35c = 1;
  func_001C57A8(&D_004B25A0.x4f0, (u128)0x5443454c45535f53UL);
 } else {
  switch ((*(short *)((char *)obj + index * 16 + 0x1c) == 0x46)) {
  case 1:
  D_00398C40.x5be4 = 0;
  D_00398C40.x5be8 = 0;
  func_00148560(D_0037EA60, 1);
  break;
  }
 }
 state = D_003F52D0;
 if (state->x134) state->x134->x4 = 0;
 state->x12d = 1;
}
