/* UNIT 0x002068E8 0x00206A20 */
typedef struct List { unsigned short count; char pad[14]; void *items[1]; } List;
typedef struct Entry { char pad[0x20]; List *x20; void *x24; } Entry;
typedef struct Group { int x0; unsigned int count; Entry *items[1]; } Group;
typedef struct Arena { int x0; int offset; char pad8[8]; } Arena;
extern Group *D_004D0108[];
extern Arena D_00663B08;
extern void *func_001FFB70(int, int);
extern void *func_00206A40(void *, Entry *);
extern void func_002067D8(Entry *);
extern void func_002066F0(void *, Entry *, unsigned int);
void func_002068E8(Group *arg0, int arg1) {
 unsigned int i, j;
 Entry *entry;
 int offset;
 D_004D0108[arg1] = arg0;
 for (i=0; i<D_004D0108[arg1]->count; i++) {
  entry = D_004D0108[arg1]->items[i];
  offset = (D_00663B08.offset + 15) & ~15;
  D_00663B08.offset = offset + 24;
  entry->x24 = func_00206A40(func_001FFB70(24, offset), entry);
  func_002067D8(entry);
  for (j=0; j<entry->x20->count; j++) {
   if (entry->x20->items[j]) func_002066F0(entry->x20->items[j], entry, j);
  }
 }
}
