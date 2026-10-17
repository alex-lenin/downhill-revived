/* UNIT 0x001DE3E0 0x001DE730 .rodata=0x293660:0x293880 .sdata=0x29D648:0x29D678 */
extern void func_001DF560(void *obj, const char *name, int *out);
extern char D_0043DCD2[];

typedef struct {
    char pad0[0xC8];
    int xC8;
    int vag[40];
    char pad16C[0x14];
    int x180;
} Resources;

void func_001DE3E0(Resources *res)
{
    func_001DF560(res, "CTDH.VPK", &res->vag[0]);
    func_001DF560(res, "ETDH.VPK", &res->vag[1]);
    func_001DF560(res, "GTDH.VPK", &res->vag[2]);
    func_001DF560(res, "OTDH.VPK", &res->vag[3]);
    func_001DF560(res, "PTDH.VPK", &res->vag[4]);
    func_001DF560(res, "T1TDH.VPK", &res->vag[5]);
    func_001DF560(res, "T2TDH.VPK", &res->vag[6]);
    func_001DF560(res, "TTDH.VPK", &res->vag[7]);
    func_001DF560(res, "E2TDH.VPK", &res->vag[8]);
    func_001DF560(res, "HHTDH.VPK", &res->vag[9]);
    func_001DF560(res, "T2MX.VPK", &res->vag[10]);
    func_001DF560(res, "OMX.VPK", &res->vag[11]);
    func_001DF560(res, "CMX.VPK", &res->vag[12]);
    func_001DF560(res, "T1MX.VPK", &res->vag[13]);
    func_001DF560(res, "PMX.VPK", &res->vag[14]);
    func_001DF560(res, "GMX.VPK", &res->vag[15]);
    func_001DF560(res, "TMX.VPK", &res->vag[16]);
    func_001DF560(res, "EMX.VPK", &res->vag[17]);
    func_001DF560(res, "E2MX.VPK", &res->vag[18]);
    func_001DF560(res, "HHMX.VPK", &res->vag[19]);
    func_001DF560(res, "01_LIC.VPK", &res->vag[20]);
    func_001DF560(res, "02_LIC.VPK", &res->vag[21]);
    func_001DF560(res, "03_LIC.VPK", &res->vag[22]);
    func_001DF560(res, "04_LIC.VPK", &res->vag[23]);
    func_001DF560(res, "05_LIC.VPK", &res->vag[24]);
    func_001DF560(res, "06_LIC.VPK", &res->vag[25]);
    func_001DF560(res, "07_LIC.VPK", &res->vag[26]);
    func_001DF560(res, "08_LIC.VPK", &res->vag[27]);
    func_001DF560(res, "09_LIC.VPK", &res->vag[28]);
    func_001DF560(res, "10_LIC.VPK", &res->vag[29]);
    func_001DF560(res, "11_LIC.VPK", &res->vag[30]);
    func_001DF560(res, "12_LIC.VPK", &res->vag[31]);
    func_001DF560(res, "13_LIC.VPK", &res->vag[32]);
    func_001DF560(res, "14_LIC.VPK", &res->vag[33]);
    func_001DF560(res, "15_LIC.VPK", &res->vag[34]);
    func_001DF560(res, "16_LIC.VPK", &res->vag[35]);
    func_001DF560(res, "17_LIC.VPK", &res->vag[36]);
    func_001DF560(res, "18_LIC.VPK", &res->vag[37]);
    func_001DF560(res, "19_LIC.VPK", &res->vag[38]);
    func_001DF560(res, "20_LIC.VPK", &res->vag[39]);
    res->xC8 = 0;
    res->x180 = D_0043DCD2[0];
}
