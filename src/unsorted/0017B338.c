/* UNIT 0x0017B338 0x0017B3C0 */
extern int D_0026EC78[];
extern void func_00254544(int,int);
void func_0017B338(char *obj,int id,int priority,int flag,int offset,int value) {
 if (*(int *)(obj+0x12F4) >= priority) {
  *(int *)(obj+0x12F4)=priority;
  *(int *)(obj+0x12F0)=180;
  *(int *)(obj+0x12E8)=id;
  *(int *)(obj+0x12A4)=0;
  if (flag) {
   *(int *)(obj+0x12EC)=1;
   *(int *)(obj+0x1308)=0;
   *(int *)(obj+0x1304)=0;
  } else *(int *)(obj+0x12EC)=0;
 } else {
  *(int *)(obj+0x12FC)=flag;
  *(int *)(obj+0x1300)=priority;
  *(int *)(obj+0x12F8)=id;
 }
 if (offset>0) func_00254544(D_0026EC78[id]+offset,value);
}
