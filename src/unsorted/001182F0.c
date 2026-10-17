/* UNIT 0x001182F0 0x00118380 */
extern void func_00117598(void *, int);
extern void func_001179D0(void *);
void func_001182F0(char *obj) {
 func_00117598(obj, 0);
 func_001179D0(obj + 0x10070);
 *(float *)(obj+0x1003c)=0.75f;
 *(float *)(obj+0x10040)=0.375f;
 *(float *)(obj+0x10044)=0.3f;
 *(float *)(obj+0x10048)=0.0008f;
 *(int *)(obj+0x10888)=0;
 obj[0x1089c]=0;
 obj[0x1089e]=0;
 *(int *)(obj+0x10894)=0;
 *(int *)(obj+0x10898)=0;
 obj[0x1089f]=0;
}
