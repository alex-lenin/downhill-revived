/* UNIT 0x00143330 0x00143390 */
typedef struct Entry { char data[0x170]; } Entry;
typedef struct Obj { int x0; int count; char pad[0x68]; Entry entries[1]; } Obj;
extern void func_00141D10(Entry *);
void func_00143330(Obj *obj) {
 int i;
 for (i=0; i<obj->count; ++i) func_00141D10(&obj->entries[i]);
}
