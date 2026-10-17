/* UNIT 0x00230BD0 0x00230C80 */
typedef struct { char pad[0x84]; void *x84; int x88,x8C,x90; } Entry;
extern void func_00230CA0(void *);
extern int func_0020D930(void *,void *);
extern void func_00230A70(void *,void *);
extern void func_002308A0(void *,void *);
extern int func_0020DB78(void *,void *,int);
void func_00230BD0(void *obj) {
    Entry *entry = (Entry *)((char *)obj+0x8000);
    entry->x84=obj;
    if(entry->x8C) {
        func_00230CA0(obj);
        if(func_0020D930(entry->x84,entry)>0) func_00230A70(obj,entry->x84);
        entry->x8C=0;
        if(entry->x90) entry->x90=0;
    } else if(entry->x90) {
        func_00230CA0(obj);
        func_002308A0(obj,entry->x84);
        func_0020DB78(entry,entry->x84,0x4000);
        entry->x90=0;
    }
}
