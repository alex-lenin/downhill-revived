/* UNIT 0x001BE5B0 0x001BE628 */
typedef struct { int x0[2]; int values[31]; int x84; int x88; } Obj;
typedef struct { char pad[0x3244]; int count; } Global;
extern Global D_0032CA00;
int func_001BE5B0(Obj *obj) {
 int i;
 obj->x84=0; obj->x88=0;
 for (i=0;i<D_0032CA00.count;i++) {
 if (obj->values[i]<obj->values[obj->x84]) {
 obj->x88=obj->x84; obj->x84=i;
 }
 }
 return obj->x84;
}
