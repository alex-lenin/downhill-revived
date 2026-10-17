/* UNIT 0x0010E150 0x0010E1E0 */
typedef struct Obj { char pad[0x3a0]; int x3a0; char pad3a4[0x68]; int x40c; char pad410[0x38]; long x448; } Obj;
extern int func_00111A98(Obj *);
void func_0010E150(Obj *obj) {
 int value, limit; long flags;
 value=obj->x40c;
 if(value>=0x1c21) limit=2; else limit=value>=0xa8d?3:4;
 if(((obj->x3a0 >> 11)&1) || limit>=func_00111A98(obj)) obj->x448 |= 1; else obj->x448 &= ~1L;
}
