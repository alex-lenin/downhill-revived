/* UNIT 0x00113D10 0x00113EB8 */
#define FIELD(p,t,o) (*(t *)((char *)(p)+(o)))
extern int func_00138560(void *);
extern void func_00221888(void *,void *,float *);
extern int func_00114430(void *);
extern void func_00116368(void *,int);
static __inline__ void set2(void *p,int value) {
 if(FIELD(p,unsigned char,0x13da)) FIELD(p,int,0x1264)=value;
 else { FIELD(p,int,0x119c)=value; FIELD(p,int,0x1264)=0; }
}
static __inline__ void set1(void *p,int value) {
 if(FIELD(p,unsigned char,0x13d9)) FIELD(p,int,0x1260)=value;
 else { FIELD(p,int,0x1198)=value; FIELD(p,int,0x1260)=0; }
}
void func_00113D10(void *obj) {
 float v[4]; int stop=0; int active; void *p;
 if ((FIELD(obj,unsigned long,0x440)&(0x8000UL<<40)) && (func_00138560((char *)FIELD(obj,void *,0)+0x40c0)&255)) {
  func_00221888((char *)obj+0x90,(char *)FIELD(FIELD(obj,void *,0),void *,0x792c)+0x50,v);
  p=FIELD(obj,void *,0);
  if (FIELD(p,unsigned char,0x13d9)) active=FIELD(p,int,0x1260); else active=FIELD(p,int,0x11fc);
  if (active) { if(v[2]<-0.5f && v[1]>0.5f) stop=func_00114430(obj)==0; }
  else { if(v[2]>0.5f && v[1]>0.5f) stop=func_00114430(obj)==0; }
 } else stop=1;
 if(stop) {
  FIELD(obj,int,0x2d4)=4;
  FIELD(obj,unsigned long,0x440)&=~(8UL<<48);
  func_00116368((char *)obj+0x2d4,4);
  return;
 }
 FIELD(obj,float,0x2b0)=1.0f;
 if(FIELD(obj,unsigned long,0x440)&(0x8000UL<<42)) set2(FIELD(obj,void *,0),1);
 else set1(FIELD(obj,void *,0),1);
}
