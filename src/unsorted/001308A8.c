/* UNIT 0x001308A8 0x00130910 */
typedef struct Child { char pad[0x2720]; int x2720; char pad2724[8]; int x272C; } Child;
typedef struct Obj { char pad[0x7A54]; Child *x7A54,*x7A58; } Obj;
extern void func_0022DDD8(Child *);
void func_001308A8(Obj *obj) {
 if(obj->x7A54) {
 obj->x7A54->x2720=0; obj->x7A54->x272C=0;
 func_0022DDD8(obj->x7A54); obj->x7A54=0;
 }
 if(obj->x7A58) {
 obj->x7A58->x2720=0; obj->x7A58->x272C=0;
 func_0022DDD8(obj->x7A58); obj->x7A58=0;
 }
}
