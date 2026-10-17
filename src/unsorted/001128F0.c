/* UNIT 0x001128F0 0x00112A68 */
typedef struct Vec {float x,y,z,w;} Vec __attribute__((aligned(16)));
typedef struct Frame {char pad[16];Vec x10;} Frame;
typedef struct Entity {char pad[0x792c];Frame *x792C;char pad7930[72];int x7978;char pad797C[16];int x798C;} Entity;
typedef struct Obj {Entity *x0;char pad4[4];Entity *x8;char padC[0x104];Vec x110[20];char pad250[60];float x28C,x290,x294;} Obj;
typedef struct Global {char pad[0x50130];Entity *riders[26];} Global;
typedef union Addr {Global *ptr; unsigned int value;} Addr;
typedef struct World {char pad[0x288];int count;} World;
extern Global D_002AF2C0;
extern World D_004B25A0;
extern float D_0029C108[],D_0029C10C[];
static inline void Sub(Vec *d,Vec *a,Vec *b) {
 __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0":"=m"(*d):"r"(a),"r"(b));
}
void func_001128F0(Obj *obj) {
 unsigned int rider;
 int i;
 Vec *delta;
 World *world;
 float length,y,initial,min;
 int mode;
 Entity *other;
 Addr address;
 initial=D_0029C108[0];min=D_0029C10C[0];
 obj->x28C=initial;obj->x290=min;obj->x294=initial;obj->x8=0;
 i=0;
 if(D_004B25A0.count>i) {
 address.ptr=&D_002AF2C0;
 rider=address.value+0x50130;delta=obj->x110;
 do {
  other=*(Entity **)rider;
  Sub(delta,&other->x792C->x10,&obj->x0->x792C->x10);
  __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0":"=&f"(length):"f"(delta->x),"f"(delta->y),"f"(delta->z));
  mode=obj->x0->x798C;
  if(mode>=6 && D_004B25A0.count>=2 && obj->x8) {
   if((mode&1)==obj->x8->x7978) {
    obj->x294=length;obj->x8=other;
    obj->x28C=other->x792C->x10.z;obj->x290=other->x792C->x10.z;
   }
  } else {
   y=other->x792C->x10.z;
   if(y<obj->x28C) obj->x28C=y;
   y=other->x792C->x10.z;
   if(obj->x290<y) obj->x290=y;
   if(length<obj->x294) {obj->x294=length;obj->x8=other;}
  }
  delta++;rider+=4;
 } while(++i<D_004B25A0.count);
 }
}
