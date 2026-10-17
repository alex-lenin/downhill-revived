/* UNIT 0x0018EE18 0x0018EE80 */
typedef struct { char pad[0x4B9]; char x4B9; } Entry;
typedef struct { char pad[0x500]; Entry *entries[12]; int count; char x534[0x1C]; char x550[1]; } Obj;
extern void func_0018E4C0(Entry *,void *);
extern void func_0018E4D8(Entry *,void *);
void func_0018EE18(Obj *obj,Entry *entry) {
 obj->entries[obj->count]=entry;
 entry->x4B9=0;
 func_0018E4C0(entry,obj->x534);
 func_0018E4D8(entry,obj->x550);
 obj->count++;
}
