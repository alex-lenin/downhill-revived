/* UNIT 0x0013EC38 0x0013EF28 */
typedef struct { char pad[0x5bec]; signed char x5bec; char pad5bed[3]; signed char x5bf0; char pad5bf1[3]; signed char x5bf4; } Data;
extern Data D_00398C40;
extern void func_0013F600(void *,int,int,int,int,int,int);
void func_0013EC38(void *arg0) {
 Data *d=&D_00398C40;
 func_0013F600(arg0, 0x2710, 0x3B, 0x6C, 5, 1, d->x5bf4);
 func_0013F600(arg0, 0x2710, 0x37, 0x6D, 5, 1, d->x5bf0);
 func_0013F600(arg0, 0x2710, 0x42, 0x6D, 5, 1, d->x5bec);
 func_0013F600(arg0, 0xC350, 0x43, 0x6E, 5, 1, -1);
 func_0013F600(arg0, 0x1388, 0x32, 0x6A, 1, 1, -1);
 func_0013F600(arg0, 0x1388, 0x33, 0x6A, 1, 2, -1);
 func_0013F600(arg0, 0x1388, 0x34, 0x6A, 1, 3, -1);
 func_0013F600(arg0, 0x3A98, 0x38, 0x69, 3, 1, -1);
 func_0013F600(arg0, 0x3A98, 0x39, 0x69, 3, 2, -1);
 func_0013F600(arg0, 0x3A98, 0x3A, 0x69, 3, 3, -1);
 func_0013F600(arg0, 0x2710, 0x2D, 0x68, 0, 1, -1);
 func_0013F600(arg0, 0x2710, 0x2E, 0x68, 0, 2, -1);
 func_0013F600(arg0, 0x2710, 0x2F, 0x68, 0, 3, -1);
 func_0013F600(arg0, 0x2710, 0x30, 0x68, 0, 4, -1);
 func_0013F600(arg0, 0x2710, 0x31, 0x68, 0, 5, -1);
 func_0013F600(arg0, 0x4E20, 0x3C, 0x6B, 4, 1, -1);
 func_0013F600(arg0, 0x4E20, 0x35, 0x6D, 2, 1, -1);
 func_0013F600(arg0, 0x4E20, 0x36, 0x6D, 2, 1, -1);
 func_0013F600(arg0, 0x4E20, 0x46, 0x6F, 6, 1, -1);
 func_0013F600(arg0, 0x61A8, 0x44, 0x6F, 6, 2, -1);
 func_0013F600(arg0, 0x7530, 0x45, 0x6F, 6, 3, -1);
 func_0013F600(arg0, 0xC350, 0x47, 0x6F, 6, 1, -1);
}
