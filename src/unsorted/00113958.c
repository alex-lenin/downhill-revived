/* UNIT 0x00113958 0x001139F0 .rodata=0x288950:0x288970 */
typedef struct Obj { char pad[0x2d4]; int x2D4; char pad2[0x410-0x2d8]; int x410; } Obj;
extern int func_00238F60(void);
extern void func_00117218(void *);
extern void func_001139F0(Obj *);
extern void func_00113D10(Obj *);
extern void func_00113EB8(Obj *);
extern void func_001140A0(Obj *);
void func_00113958(Obj *obj) {
 obj->x410+=func_00238F60();
 func_00117218(&obj->x2D4);
 switch(obj->x2D4) {
 case 0: case 1: func_001139F0(obj); break;
 case 2: func_00113D10(obj); break;
 case 3: func_00113EB8(obj); break;
 case 4: default: func_001140A0(obj); break;
 }
}
