/* UNIT 0x0019B350 0x0019B638 */
typedef unsigned int u128 __attribute__((mode(TI)));
typedef struct Menu { int x0; char p0[0x418-4]; int x418; char p1[0x45c-0x41c]; int x45C; char p2[0x504-0x460]; int x504,x508; } Menu;
typedef struct State { char p0[0x500]; int x500; char p1[8]; signed char x50C[52]; signed char x540[16]; } State;
extern State D_0043CD50;
typedef struct Input { char pad[0x64]; char keys[0x100]; } Input;
extern Input D_003F52D0;
extern char D_0043D7B0[],D_004B2A90[],D_0028E588[],D_0028E598[];
extern char *D_0026F7F8[];
extern void func_00190358(void *);
extern int func_00190D60(void *,int,int);
extern void func_00190660(void *,void *);
extern void func_0019AB50(void *,int);
extern void func_001C57A8(void *,u128);
extern void func_0019B000(void *,int);
extern void func_0018E408(Menu *,void *,int);
extern void func_0019AB60(void *);
extern void func_0018DC20(Menu *);
#define SOUND_OK ((u128)0x5443454c45535f53UL)
#define SOUND_BAD ((u128)0x4c45434e41435f53UL)
void func_0019B350(Menu *obj) {
 Input *base=&D_003F52D0; char *input; int i,count,index,row; signed char value; signed char *items,*counts; State *state;
 func_00190358(base); input=base->keys;
 if(func_00190D60(input,6,obj->x45C)) {
 if(D_0043CD50.x540[D_0043CD50.x500]<12) {
 obj->x504=obj->x0; obj->x508=0;
 func_00190660(base,D_0043D7B0);
 func_001C57A8(D_004B2A90,SOUND_OK);
 } else {
 func_0019AB50(&D_0043CD50,2);
 func_001C57A8(D_004B2A90,SOUND_BAD);
 }
 } else if(func_00190D60(input,7,obj->x45C) && D_0043CD50.x540[D_0043CD50.x500]>0 && D_0043CD50.x50C[obj->x0+D_0043CD50.x500*13]!=40) {
 obj->x504=obj->x0; obj->x508=1;
 func_00190660(base,D_0043D7B0);
 func_001C57A8(D_004B2A90,SOUND_OK);
 } else if(func_00190D60(input,10,obj->x45C)) {
 state=&D_0043CD50; counts=state->x540;
 if(counts[state->x500]>0) {
 index=obj->x0; row=state->x500*13; items=state->x50C;
 if(items[index+row]!=40 && index<counts[state->x500]) {
 i=0;
 func_001C57A8(D_004B2A90,SOUND_OK);
 func_0019B000(state,obj->x0); obj->x418=0;
 count=counts[state->x500]+1;
 for(;i<count;) {
 if(count==1) func_0018E408(obj,D_0028E588,1);
 else { value=D_0043CD50.x50C[D_0043CD50.x500*13+i];
 if(value==40) func_0018E408(obj,D_0028E598,1);
 else func_0018E408(obj,D_0026F7F8[value],1);
 }
 i++;
 }
 }
 }
 }
 func_0019AB60(&D_0043CD50); func_0018DC20(obj);
}
