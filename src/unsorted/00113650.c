/* UNIT 0x00113650 0x00113810 */
struct World;
struct Obj {
struct World * x0;
char pad4[924];
int x3A0;
char pad3a4[96];
int x404;
char pad408[16];
int x418;
char pad41c[36];
long x440;
};
struct World {
char pad0[4612];
int x1204;
char pad1208[96];
int x1268;
char pad126c[367];
unsigned char x13DB;
char pad13dc[15088];
char * x4ECC;
};
struct Part {
char pad0[448];
int x1C0;
int x1C4;
char pad1c8[96];
int x228;
char pad22c[92];
int x288;
int x28C;
char pad290[363];
unsigned char x3FB;
unsigned char x3FC;
};
static __inline__ int get_value(struct Part *part) { return part->x3FC ? part->x28C : part->x228; }
static __inline__ int world_value(struct World *world) { return world->x13DB ? world->x1268 : world->x1204; }
static __inline__ void set_a(struct Part *part, int value) { if(part->x3FB) part->x288=value; else { part->x1C0=value; part->x288=0; } }
static __inline__ void set_b(struct Part *part, int value) { if(part->x3FC) part->x28C=value; else { part->x1C4=value; part->x28C=0; } }
extern int func_00238F60(void);
void func_00113650(struct Obj *obj) {
 long flags;
 if(obj->x440 & (0x8000L<<18)) {
  if(obj->x440 & (0x8000L<<27)) {
   int count=obj->x404+func_00238F60(); obj->x404=count;
   if(count>=16) obj->x440&=~(1L<<43), obj->x440&=~(1L<<42);
   else {
    struct World *world; struct Part *part=(struct Part*)((char*)obj->x0+0xFE0);
    int v;
    v=world_value(obj->x0);
    if(v) set_a(part,1);
    else if(get_value(part)) set_b(part,1);
    world=obj->x0;
    if(world->x4ECC==(char*)world+0x5E90) {
     obj->x440&=~(1L<<33); obj->x440&=~(1L<<42);
     if((obj->x3A0>>3)&1) obj->x418=300;
    }
   }
  }
  flags=obj->x440;
  if((flags&(0xC000L<<28))==(0x8000L<<28)) {
   struct World *world=obj->x0;
   if(world->x4ECC!=(char*)world+0x5E90) obj->x440=flags&~(1L<<43);
  }
 }
}
