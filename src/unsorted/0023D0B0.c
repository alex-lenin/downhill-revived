/* UNIT 0x0023D0B0 0x0023D180 */
typedef struct Entry { int x0; int **x4; int **x8; int **xC; char pad[0xe0]; } Entry;
extern Entry D_00528360[];
extern int *D_0029E448,*D_0029E44C;
extern int **func_00208FB8();
void func_0023D0B0(int *obj,int index) {
 int flags=*obj&0x3ff80;
 Entry *entry;
 if(!flags) D_0029E448=obj;
 else if(flags==0x80) D_0029E44C=obj;
 entry=&D_00528360[index];
 if(!entry->x4) entry->x4=func_00208FB8();
 if(!entry->xC) entry->xC=func_00208FB8();
 if(!(*obj&0x3ff80)) *entry->x4=obj;
 if((*obj&0x3ff80)==0x200) *entry->xC=obj;
}
