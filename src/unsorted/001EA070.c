/* UNIT 0x001EA070 0x001EA218 */
typedef struct Handle { int x0, x4, x8, xC; } Handle;
typedef struct Table { char pad0[0xC]; int xC; char pad10[0x38]; float x48; char pad4C[8]; } Table;
typedef struct Obj { char *x0; char pad4[0x28]; int x2C, x30, x34; char pad38[0x194]; int x1CC; } Obj;
extern Table D_00272888[], D_00273D60[];
extern float func_00203C28(Handle *);
extern float func_001E5190(void);
extern int func_00204170(Handle *);
static __inline__ Handle *get_entry(char *group, int index) { return (Handle *)(group + (index * 16 + 0x14)); }
int func_001EA070(Obj *obj) {
 Handle temp;
 char *group = obj->x0 + 0x1E40;
 Table *a = &D_00272888[obj->x2C];
 Table *b = &D_00273D60[obj->x30];
 float value, threshold; int flag;
 if (obj->x1CC != 5 && obj->x1CC != 3 && (obj->x1CC != 1 || a->xC != 0)) return 0;
 if (obj->x1CC == 3 && obj->x34 != b->xC && b->xC != 0) return 0;
 temp = *get_entry(group, obj->x34);
 value = func_00203C28(&temp);
 temp = *get_entry(group, obj->x34);
 flag = func_00204170(&temp);
 threshold = a->x48;
 if (obj->x1CC == 3) threshold = b->x48;
 threshold *= func_001E5190();
 if (flag) {
  if (!(threshold < value)) return 0;
 } else return value < 1.0f - threshold;
 return 1;
}
