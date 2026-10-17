/* UNIT 0x00193B20 0x00193BB8 */
extern struct {char pad[0x5bec]; int x5bec,x5bf0,x5bf4;} D_00398C40; extern int D_004B2900[];
extern void func_00195C40(void *); extern void func_0016C298(void *,unsigned char);
void func_00193B20(unsigned char *o){unsigned char *p;
if(!o[0x1219a])D_00398C40.x5bf0=0;
if(!o[0x121a0])D_00398C40.x5bec=0;
if(!o[0x121a6])D_00398C40.x5bf4=0;
if(!o[0x12198])D_004B2900[0]=0;
p=o+0x11a60;func_00195C40(p);func_0016C298(&D_00398C40,p[0x14]);}
