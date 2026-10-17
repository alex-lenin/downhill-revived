/* UNIT 0x001284F0 0x001286D8 */
typedef struct PoolObj { char pad[0x230]; } PoolObj;
typedef struct Handle { char pad0[0x230]; PoolObj *x230; } Handle;
typedef struct Pool { PoolObj entries[200]; unsigned char freeIndices[200]; unsigned char freeCount; char pad1B649[7]; } Pool;
typedef struct World { char pad0[0x5dd0]; Pool pool; int count; char pad21424[0x14]; int counts[100]; int handles[100][24]; int active[100]; } World;
extern Handle *func_00193498(int);
extern void func_00122CF0(PoolObj *);
extern void func_001933A0(Handle *);
static inline void Release(Pool *pool,PoolObj *obj) {
 int index;
 func_00122CF0(obj);
 index=obj-pool->entries;
 pool->freeIndices[--pool->freeCount]=index;
}
void func_001284F0(World *world,int id) {
 int i,n,*p; Handle *handle; PoolObj *obj;
 if(world->count>=2) {
  for(i=0;i<world->count;i++) if(world->active[i]==id) break;
  for(;i<world->count-1;i++) world->active[i]=world->active[i+1];
 }
 world->count--;
 *(int *)((char *)world+id*0xf0+0xf0)=1;
 n=world->counts[id];
 p=world->handles[id];
 while(n) {
  n--;
  handle=func_00193498(*p++);
  obj=handle->x230;
  Release(&world->pool,obj);
  func_001933A0(handle);
 }
 world->counts[id]=0;
}
