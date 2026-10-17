/* UNIT 0x00190528 0x00190608 */
typedef struct VTable { char pad[0x70]; short delta; short pad72; void (*call)(void *); } VTable;
typedef struct Obj { char pad[0x480]; int x480; char pad484[0x28]; void *x4AC; char pad4B0[9]; char x4B9; char pad4BA[0x3E]; VTable *vt; } Obj;
typedef struct State { char pad[0x14C]; int x14C; Obj *x150; } State;
typedef struct Node { char pad[13]; char xD; } Node;
extern int func_0018DB10(Obj *, int);
extern int func_00238F48(void);
extern void func_00190448(State *, Obj *);
extern void func_0018D7B0(void *);
extern Node *func_0018D718(Obj *, Obj *, int);
extern void func_0018E4C0(Obj *, Node *);
void func_00190528(State *state, Obj *obj, int notify) {
 Obj *old; Node *node; VTable *vt;
 if (state->x150 != obj) {
  if (obj && func_0018DB10(obj,state->x14C)) {
   vt=obj->vt; vt->call((char *)obj+vt->delta);
   obj->x480=func_00238F48();
  }
  old=state->x150;
  func_00190448(state,obj);
  if(notify) {
   if(obj->x4AC) func_0018D7B0(obj->x4AC);
   node=func_0018D718(state->x150,old,0);
   node->xD=1;
   func_0018E4C0(obj,node);
  }
  if(obj) obj->x4B9=0;
 }
}
