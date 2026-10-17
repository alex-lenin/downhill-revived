/* UNIT 0x001DAC50 0x001DAEE8 */
typedef struct Obj { char pad[0xA98]; int count1,count2,index1,index2; } Obj;
extern struct State { char pad[0x2A0]; volatile int level; } D_004B25A0;
extern void func_001D2CB8(void *),func_001C0E00(void *),func_001D4C58(void *),func_001DB510(void *),func_001C0B60(void *),func_001D1C58(void *);
extern void func_001BEA48(void *,float *);
void func_001DAC50(Obj *obj) {
 float pos[3]; int i,level=D_004B25A0.level; void *a,*b,*c,*d;
 if(level==20) {
  func_001D2CB8((char *)obj+0x40);
  func_001D2CB8((char *)obj+0x70);
  func_001D2CB8((char *)obj+0xA0);
  func_001D2CB8((char *)obj+0xD0);
  func_001D2CB8((char *)obj+0x100);
  func_001D2CB8((char *)obj+0x130);
  func_001D2CB8((char *)obj+0x160);
  func_001D2CB8((char *)obj+0x190);
  func_001D2CB8((char *)obj+0x1C0);
  func_001D2CB8((char *)obj+0x1F0);
  func_001D2CB8((char *)obj+0x280);
  func_001C0E00((char *)obj+0xA70); func_001C0E00((char *)obj+0xA84);
  pos[0]=-2997.0f; pos[1]=-6947.0f; pos[2]=-6431.0f; func_001BEA48((char *)obj+0xAB0,pos);
 } else if(level==17) {
  func_001D2CB8((char *)obj+0x40);
  func_001D2CB8((char *)obj+0x70);
  func_001D2CB8((char *)obj+0xA0);
  func_001D2CB8((char *)obj+0x130);
  func_001D2CB8((char *)obj+0x160);
  func_001D2CB8((char *)obj+0x190);
  func_001D2CB8((char *)obj+0x1F0);
 } else if(level==18) {
  func_001D2CB8((char *)obj+0x10);
  func_001D2CB8((char *)obj+0x40);
  func_001D2CB8((char *)obj+0x70);
  func_001D2CB8((char *)obj+0xA0);
  func_001D2CB8((char *)obj+0xD0);
  func_001D2CB8((char *)obj+0x100);
  func_001D2CB8((char *)obj+0x130);
  func_001D2CB8((char *)obj+0x160);
  func_001D2CB8((char *)obj+0x190);
  func_001D2CB8((char *)obj+0x1C0);
  func_001D2CB8((char *)obj+0x1F0);
  func_001D2CB8((char *)obj+0x220);
  func_001D2CB8((char *)obj+0x250);
 }
 if(D_004B25A0.level==18) func_001D4C58(obj);
 a=(char *)obj+0x2B0; b=(char *)obj+0x2E0; c=(char *)obj+0x350; d=(char *)obj+0x3C0;
 if(obj->count1>0) {
  for(i=1;i>=0;i--) {
   func_001DB510((char *)obj+(obj->index1*64+0x430));
   obj->index1++; if(obj->index1>=obj->count1) obj->index1=0;
  }
 }
 if(obj->count2>0) {
  func_001C0B60((char *)obj+(obj->index2*0x30+0x9B0));
  obj->index2++; if(obj->index2>=obj->count2) obj->index2=0;
 }
 func_001D2CB8(a); func_001D1C58(b); func_001D1C58(c); func_001D1C58(d);
}
