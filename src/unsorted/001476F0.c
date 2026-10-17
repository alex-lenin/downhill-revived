/* UNIT 0x001476F0 0x00147750 */
typedef struct Entry { char data[0x10]; } Entry;
typedef struct Obj { Entry entries[30]; int count; } Obj;
extern void func_00147920(Entry *);
void func_001476F0(Obj *obj) {
 int i;
 for (i=0;i<obj->count;++i) func_00147920(&obj->entries[i]);
}
