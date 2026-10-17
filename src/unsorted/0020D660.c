/* UNIT 0x0020D660 0x0020D6F8 */
typedef struct { char pad[8]; int active; char padC[9]; char name[0x33]; } Entry;
typedef struct { Entry entries[80]; int count; } Obj;
extern int strcmp(const char *,const char *);
int func_0020D660(Obj *obj,const char *name) {
 int i;
 for(i=0;i<obj->count;i++) {
  if(strcmp(name,obj->entries[i].name)==0 && obj->entries[i].active>0) return i;
 }
 return -1;
}
