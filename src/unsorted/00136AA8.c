/* UNIT 0x00136AA8 0x00136B40 */
typedef struct { char pad[0x394]; unsigned short x394; unsigned short type : 5; unsigned short x396_5 : 11; char pad398[0xDC]; float x474; char pad478[0x38]; long x4B0; } Obj;
typedef struct { char pad[0xC]; float xC; char pad10[0x10]; } Entry;
extern Entry D_00768098[];
extern int func_00138560(Obj *);
extern int func_00138570(Obj *);

static inline Entry *GetEntry(int type) {
 if (type >= 32) return D_00768098;
 return &D_00768098[type];
}

void func_00136AA8(Obj *o) {
 int type;
 if ((o->x4B0 & 2) && !func_00138560(o)) {
  type = o->type;
  if ((unsigned)(type - 24) < 3 && func_00138570(o)) {
   o->x474 = GetEntry(type)->xC;
  }
 }
}
