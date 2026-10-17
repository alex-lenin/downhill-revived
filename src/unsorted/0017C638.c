/* UNIT 0x0017C638 0x0017C6B0 */
extern void func_0017C168(void *);
void func_0017C638(char *obj) {
 if (*(int *)(obj+0xF84)) {
  int state;
  if (*(int *)(obj+0x1310)==1) (*(char **)(obj+0x117C))[4]=0;
  func_0017C168(obj);
  state=*(int *)(obj+0x1310);
  if (state==60) {
   *(int *)(obj+0x1318)=state;
   *(float *)(obj+0x1288)=-*(float *)(obj+0x1288)*0.5f;
  }
 }
}
