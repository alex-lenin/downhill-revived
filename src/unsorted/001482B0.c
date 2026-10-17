/* UNIT 0x001482B0 0x00148310 */
typedef struct Entry { char data[0x380]; } Entry;
typedef struct Obj { Entry entries[4]; int count; } Obj;
extern void func_00148BE8(Entry *,int);
void func_001482B0(Obj *obj) {
 int i;
 for (i=0;i<obj->count;++i) func_00148BE8(&obj->entries[i],i);
}
