/* UNIT 0x0012F6A0 0x0012F7D0 */
typedef struct {
 char pad0[0x1180]; int x1180;
 char pad1184[8]; int x118C;
 char pad1190[0xB8]; int x1248;
 char pad124C[8]; int x1254;
 char pad1258[0x17B]; unsigned char x13D3;
 char pad13D4[2]; unsigned char x13D6;
 char pad13D7[0x2CE9]; char x40C0[0x4C0]; int x4580;
 char pad4584[0x33E2]; unsigned short x7966;
 unsigned short x7968, x796A;
 char pad796C[0xFD]; unsigned char x7A69;
 char pad7A6A[0xA]; int x7A74; float x7A78;
} Obj;
extern float func_001385A8(void *);
extern int func_00138560(void *);
extern int func_00238F60(void);
void func_0012F6A0(Obj *obj) {
 int ticks, a, b;
 if(obj->x7A74) {
 if(--obj->x7A74 < 0 || func_001385A8(obj->x40C0) > 0.75f) obj->x7A74 = 0;
 }
 if((unsigned char)func_00138560(obj->x40C0) && !obj->x7A69) obj->x7A78 = 0.0f;
 else if(obj->x7A74) obj->x7A78 = 5.0f;
 else {
 a = obj->x13D3 ? obj->x1248 : obj->x1180;
 if(a) obj->x7A78 = 1.0f;
 else {
 b = obj->x13D6 ? obj->x1254 : obj->x118C;
 if(b) obj->x7A78 = 1.0f;
 else obj->x7A78 = 0.0f;
 }
 }
 if(obj->x4580 == 1) obj->x7A78 = 0.0f;
 if(obj->x7A78 > 0.0f) {
 ticks=func_00238F60();
 obj->x7966 = obj->x7966 + ticks;
 obj->x796A = 0;
 } else {
 obj->x7966 = 0;
 ticks=func_00238F60();
 obj->x796A = obj->x796A + ticks;
 }
}
