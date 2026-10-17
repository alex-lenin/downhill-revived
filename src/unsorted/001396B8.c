/* UNIT 0x001396B8 0x00139710 */
typedef struct Obj { char pad[0x316F8]; int x316F8; } Obj;
unsigned int func_001396B8(Obj *obj, int type, int value, float *out) {
 unsigned int color=0x80086008;
 *out=0.75f;
 if(type!=6) { if(type==7) color=0x80606008; }
 else color=0x80080860;
 return obj->x316F8 >= value ? color : 0x80303030;
}
