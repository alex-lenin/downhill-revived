/* UNIT 0x0016C2A0 0x0016C348 */
typedef struct { char pad[0x5bdc]; int x5bdc; } Obj;
typedef struct { char pad[0x78d4]; void *x78d4; char gap[0x184]; int x7a5c; char tail[0x170]; } Entry;
typedef struct { Entry entries[10]; char gap[0x2b88]; int count; } Globals;
typedef struct { char pad[0x1a8]; int count; } Count;
extern char D_0029EA20[];
extern Globals D_002AF2C0;
extern void func_0016C348(Obj *);
extern void func_001183E0(void *);
extern void func_0010B0E8(void *);
void func_0016C2A0(Obj *obj, int value) {
 int i;
 obj->x5bdc = value;
 func_0016C348(obj);
 func_001183E0(D_0029EA20);
 for (i = 0; i < D_002AF2C0.count; i++) {
  Entry *entry = &D_002AF2C0.entries[i];
  if (entry->x7a5c == 3 && entry->x78d4) func_0010B0E8(entry->x78d4);
 }
}
