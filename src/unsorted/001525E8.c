/* UNIT 0x001525E8 0x00152768 */
typedef struct {int x0,x4,x8,xC;} Entry;
typedef struct {char pad[0x58]; short delta; short pad5A; int (*call)(void *);} Vtable;
typedef struct {int x0; Vtable *vt;} Target;
typedef struct {char pad[8]; Target *x8;} Link;
typedef struct {char pad[0x14]; Entry entries[0x214]; char pad2154[0xC]; unsigned char x2160;} Group;
typedef struct {Entry *x0; int x4; float x8,xC; int x10,x14; unsigned char x18,x19,x1A,pad1B; float x1C; int x20; Link *x24;} Obj;
extern void func_00203E10(Entry *,float);
extern void func_00202E38(Entry *,int,int,float);
static __inline__ Entry *get_entry(Group *group, int index) { return (Entry *)((char *)group+(index*16+0x14)); }
static __inline__ Entry *get_base(Entry *base, int index) { return base+index; }
void func_001525E8(Obj *obj) {
 Entry *base=obj->x0; Group *group=(Group *)((char *)base+0x1E40);
 Target *target=obj->x24->x8; int index;
 Entry temp;
 index=target->vt->call((char *)target+target->vt->delta);
 obj->x14=index;
 if(index) {
 if(group->x2160) {
 temp=*get_entry(group,index); func_00203E10(&temp,obj->x8);
 func_00202E38(&group->entries[obj->x14],1,7,obj->xC);
 temp=*get_base(base,obj->x14); func_00203E10(&temp,obj->x8);
 func_00202E38(&base[obj->x14],1,7,obj->xC);
 } else {
 temp=*get_entry(group,index); func_00203E10(&temp,obj->x8);
 func_00202E38(&group->entries[obj->x14],1,1,obj->x1C);
 }
 obj->x1A=0;obj->x18=0;obj->x19=0;
 }
}
