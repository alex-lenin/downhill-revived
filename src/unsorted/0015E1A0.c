/* UNIT 0x0015E1A0 0x0015E238 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef union Vec { u128 bits; float f[4]; } Vec;
typedef struct Item { char pad[4]; char x4; } Item;
typedef struct State { char pad[0x64]; char x64[0xC9]; char x12D; char pad12E[6]; Item *x134; } State;
typedef struct Obj { char pad[0x45C]; int x45C; } Obj;
extern State D_003F52D0;
extern char D_004B28FC[];
extern void func_0018FFE0(State *,Item *,Vec *);
extern int func_00190D60(void *,int,int);
extern void func_0018DC20(Obj *);
void func_0015E1A0(Obj *obj) {
 Vec vec; State *state=&D_003F52D0;
 vec.bits=0; vec.f[1]=35.0f; vec.f[2]=16.0f;
 func_0018FFE0(state,state->x134,&vec);
 if(func_00190D60(state->x64,8,obj->x45C)) {
  D_004B28FC[0]=1;
  if(state->x134) state->x134->x4=0;
  state->x12D=1;
 }
 func_0018DC20(obj);
}
