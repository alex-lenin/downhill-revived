/* UNIT 0x0013B9C0 0x0013BA60 */
typedef struct Global { char pad[0x10c]; unsigned int x10c; char pad110[0x180]; int x290; } Global;
extern Global D_004B25A0;
typedef struct Other { char pad[0x1218c]; unsigned char x1218c[3]; } Other;
extern Other D_004225A0;
extern char D_003F52D0[], D_003638A0[];
extern void func_00190660(void *,void *);
void func_0013B9C0(void) {
 int flag;
 unsigned int index = D_004B25A0.x10c;
 if (index < 3U) {
  flag=0;
  if (D_004B25A0.x290==0xe0 || D_004B25A0.x290==0x100 || D_004B25A0.x290==0x140 || D_004B25A0.x290==0x180 || D_004B25A0.x290==0x1a0) flag=1;
  if (!flag && D_004225A0.x1218c[index]) func_00190660(D_003F52D0,D_003638A0);
 }
}
