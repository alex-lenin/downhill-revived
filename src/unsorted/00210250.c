/* UNIT 0x00210250 0x002102A8 */
typedef struct Object { int flags; char pad[0x4c]; long x50, x58; } Object;
typedef struct Command { long value, type; } Command;
extern Command *D_004F35B0[];
void func_00210250(Object *obj) {
 int count;
 Command **p;
 long type6, type20;
 if (!(obj->flags & 0x3f)) {
  type6 = 6;
  p = D_004F35B0;
  type20 = 20;
  count = 3;
  do {
   Command *cmd = *p;
   count--; p++;
   cmd->value = obj->x50; cmd->type = type6;
   cmd++;
   cmd->value = obj->x58; cmd->type = type20;
  } while (count >= 0);
 }
}
