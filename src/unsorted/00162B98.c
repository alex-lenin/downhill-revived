/* UNIT 0x00162B98 0x00162C20 */
typedef struct Input { int x0,count; } Input;
typedef struct Entry { char pad[32]; } Entry;
extern Entry D_00768098[];
extern int func_001629D0(void *,void *,int,int);
int func_00162B98(Input *p) {
 int i,offset=8;
 for(i=0;i<p->count;i++) offset+=func_001629D0(&D_00768098[i],(char *)p+offset,p->x0,i);
 return offset;
}
