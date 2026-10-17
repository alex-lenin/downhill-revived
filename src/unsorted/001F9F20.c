/* UNIT 0x001F9F20 0x001F9F98 */
typedef struct { int x0,x4,x8; } Entry;
int func_001F9F20(char *obj,Entry *key) {
 int group,i,count; Entry *p;
 obj+=8;
 for(group=0;group<6;group++,obj+=0xA0) {
 count=*(int *)(obj+0x24);
 for(i=0;i<count;i++) {
 p=(Entry *)obj+i;
 if(p->x4==key->x4 && p->x0==key->x0 && p->x8==key->x8) return 1;
 }
 }
 return 0;
}
