/* UNIT 0x00143CB0 0x00143D18 */
typedef struct { char pad[0xF80]; } Entry;
typedef struct { int count; char pad[12]; Entry entries[1]; } Obj;
extern void func_001432C0(Entry *);
extern void func_00143330(Entry *);
void func_00143CB0(Obj *o) {
 int i;
 for(i=0; i<o->count; i++) { func_001432C0(&o->entries[i]); func_00143330(&o->entries[i]); }
}
