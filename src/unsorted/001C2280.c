/* UNIT 0x001C2280 0x001C22D0 */
struct Obj { float x0,x4,x8,xC; int x10,x14,x18,x1C,x20,x24; };
extern int func_00238F48(void);
void func_001C2280(struct Obj *obj, float a,float b,float c,float d) {
 obj->x14=0; obj->x18=0; obj->x1C=0;
 obj->x0=a; obj->x4=b; obj->x8=c; obj->xC=d; obj->x10=1;
 obj->x20=func_00238F48(); obj->x24=0;
}
