/* UNIT 0x001946E0 0x00194770 */
typedef struct Obj { char pad[0x12208]; int x12208; int x1220C; } Obj;
typedef struct Global { char pad[0x288]; int count; } Global;
extern Global D_004B25A0;
extern void func_00194648(Obj *,int);
void func_001946E0(Obj *obj) {
 int i;
 int saved=obj->x12208;
 for(i=0;i<D_004B25A0.count;i++) func_00194648(obj,i);
 obj->x1220C=saved;
}
