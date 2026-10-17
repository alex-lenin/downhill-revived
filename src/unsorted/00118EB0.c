/* UNIT 0x00118EB0 0x00118F10 */
typedef struct Obj { float x0[4]; unsigned char x10,x11,x12,x13; unsigned short x14; unsigned char x16,x17; short x18[4]; } Obj;
void func_00118EB0(void *unused, Obj *obj) {
 short *p; int count;
 if(obj) {
 __asm__ volatile("sqc2 $vf0,%0" : "=m"(obj->x0));
 obj->x10=0; obj->x12=0; obj->x13=0; obj->x11=0; obj->x17=0; obj->x14=0; obj->x16=0;
 p=&obj->x18[3]; count=3;
 do { *p=-1; count--; p--; } while(count>=0);
 }
}
