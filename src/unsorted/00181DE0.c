/* UNIT 0x00181DE0 0x00181F70 */
typedef struct { int x0,x4,x8,xC; } Entry;
typedef struct { char pad[0x20]; short delta; short pad22; int (*call)(void *); } Vtable;
typedef struct { int x0; Vtable *vt; } Target;
typedef struct { char *x0; int x4,x8; } Obj;
extern void func_001AC578(void *), func_0012FE38(void *,void *);
extern unsigned char func_00204230(Entry *);
static __inline__ int check(Target *target) { return target->vt->call((char *)target+target->vt->delta); }
static __inline__ Entry *get_entry(char *group,int index) { return (Entry *)(group+(index*16+0x14)); }
void func_00181DE0(Obj *obj) {
 char *base=obj->x0; char *group=base+0x1E40;
 Target *target=(Target *)(base+0x5E40); Entry temp;
 func_001AC578(group);
 if (check(target) || check(target=(Target *)(base+0x5E70)) || check(target=(Target *)(base+0x5E90)) || check(target=(Target *)(base+0x5FF0)) || check(target=(Target *)(base+0x6210)) || check(target=(Target *)(base+0x621C)) ) { func_0012FE38(base,target); }
 else { target=(Target *)(base+0x62E0); if(check(target)) func_0012FE38(base,target); }
 temp=*get_entry(group,obj->x8);
 if ((func_00204230(&temp)^1)&0xFF) func_0012FE38(base,base+0x4ED0);
}
