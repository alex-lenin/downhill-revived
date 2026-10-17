/* UNIT 0x001E9AF8 0x001E9CC0 */
typedef struct Effect { char pad0[9]; unsigned char x9; short xa; char padc[0x44-12]; float x44; char pad48[0x64-0x48]; } Effect;
typedef struct State { char *x0; char pad4[0x2c-4]; int x2c; char pad30[12]; Effect effects[4]; } State;
typedef struct Entry { char pad0[0x2c]; float x2c,x30,x34,x38; char pad3c[0x54-0x3c]; } Entry;
extern Entry D_00272888[];
extern void func_001AC7B0(Effect *,void *);
extern void func_001AC810(Effect *,float);
extern void func_001ACB70(Effect *,float);
void func_001E9AF8(State *s,float value) {
 Entry *e=&D_00272888[s->x2c];
 char *base=s->x0;
 Effect *effect;
 if(e->x30!=0.0f) {
  effect=&s->effects[0];
  func_001AC7B0(effect,base+0x2234);
  func_001AC810(effect,value);
  func_001ACB70(effect,1.0f);
  effect->x9=4; effect->xa=999; effect->x44=e->x30;
 }
 if(e->x2c!=0.0f) {
  effect=&s->effects[1];
  func_001AC7B0(effect,base+0x2224);
  func_001AC810(effect,value);
  func_001ACB70(effect,1.0f);
  effect->x9=4; effect->xa=999; effect->x44=e->x2c;
 }
 if(e->x34!=0.0f) {
  effect=&s->effects[2];
  func_001AC7B0(effect,base+0x2394);
  func_001AC810(effect,value);
  func_001ACB70(effect,1.0f);
  effect->x9=4; effect->xa=999; effect->x44=e->x34;
 }
 if(e->x38!=0.0f) {
  effect=&s->effects[3];
  func_001AC7B0(effect,base+0x23a4);
  func_001AC810(effect,value);
  func_001ACB70(effect,1.0f);
  effect->x9=4; effect->xa=999; effect->x44=e->x38;
 }
}
