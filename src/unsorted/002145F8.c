/* UNIT 0x002145F8 0x00214760 */
extern float D_0029DA74[],D_0029DA78[];
static inline float maxf(float a,float b) { float r; __asm__ volatile("max.s %0, %1, %2" : "=&f"(r) : "f"(a),"f"(b)); return r; }
static inline float minf(float a,float b) { float r; __asm__ volatile("min.s %0, %1, %2" : "=&f"(r) : "f"(a),"f"(b)); return r; }
int func_002145F8(float *start,float *end,float *near,float *far,float *lo,float *hi,float radius) {
 float lower=D_0029DA74[0],upper=D_0029DA78[0];
 int count=3;
 start+=2; end+=2; lo+=2; hi+=2;
 do {
  float origin=*start; float delta=*end-origin;
  if(__builtin_fabsf(delta)>0.0001f) {
   float inv=1.0f/delta;
   float a=(*lo-origin-radius)*inv;
   float b=(*hi-origin+radius)*inv;
   if(a<b) { lower=maxf(lower,a); upper=minf(upper,b); }
   else { lower=maxf(lower,b); upper=minf(upper,a); }
   if(upper<lower || upper<*near || *far<lower) return 0;
  } else {
   if(origin<*lo-radius || *hi+radius<origin) return 0;
  }
  --count;
  --start; --end; --lo; --hi;
 } while(count);
 if(*near<lower) *near=lower;
 if(upper<*far) *far=upper;
 return 1;
}
