/* UNIT 0x001576D0 0x00157748 */
extern void func_001ACFA0(void *, int);
extern void func_001AD700(void *, float);
extern void func_00136030(void *, int);
extern void func_00157950(void *);
void func_001576D0(void **arg0) {
 char *obj = *arg0;
 void *part = obj + 0x4580;
 func_001ACFA0(part, 1);
 func_001AD700(part, 0.0f);
 *(int *)(obj + 0x48B0) = 1;
 *(float *)(obj + 0x7BA0) = 1.0f;
 func_00136030(obj + 0x40C0, 0);
 func_00157950((char *)arg0 + 0xE20);
}
