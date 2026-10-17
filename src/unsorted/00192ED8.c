/* UNIT 0x00192ED8 0x00192F68 */
typedef struct Obj { int x0; void *x4; void *x8; char pad[0x244]; } Obj;
extern Obj D_003F5690[];
extern int D_0029E4A0;
extern void *D_0029E4C8;
extern void func_00192A50(Obj *);
extern void func_00218278(void *,void *);
extern void func_001934B0(void);
void func_00192ED8(void) {
 int i;
 for(i=0;i<D_0029E4A0;i++) {
 func_00192A50(&D_003F5690[i]);
 func_00218278(D_003F5690[i].x8,D_0029E4C8);
 }
 func_001934B0();
}
