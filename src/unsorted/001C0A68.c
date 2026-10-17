/* UNIT 0x001C0A68 0x001C0AD0 */
extern int D_004B2840[];
extern int func_00182290(int id);

int func_001C0A68(void)
{
    switch (func_00182290(D_004B2840[0])) {
    case 2:
        return 1;
    case 0:
        return 2;
    case 1:
        return 3;
    default:
        return 0;
    }
}
