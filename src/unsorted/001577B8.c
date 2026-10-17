/* UNIT 0x001577B8 0x001577F8 */
void func_001577B8(void *obj) {
 int *p; int i; char *q;
 *(int *)((char *)obj+0x11C)=0;
 *(int *)((char *)obj+0x110)=0;
 p=(int *)((char *)obj+0x118);
 for(i=1;i>=0;i--,p--) *p=0;
 q=(char *)obj+0x10;
 *(int *)(q+0x34)=0;
 *(int *)(q+0x38)=0;
 *(int *)obj=0;
}
