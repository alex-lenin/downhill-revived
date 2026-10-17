/* UNIT 0x00112158 0x001121D8 */
typedef struct Obj { char pad[0x2b8]; float x2b8; } Obj;
extern int D_0029EA20[];
extern void func_0010B290(Obj *, int *);
int func_00112158(Obj *obj) {
 int values[2];
 values[0]=D_0029EA20[0x10894/4]; values[1]=D_0029EA20[0x10898/4];
 func_0010B290(obj,&values[0]); func_0010B290(obj,&values[1]);
 return values[1]+(int)((values[0]-values[1])*obj->x2b8);
}
