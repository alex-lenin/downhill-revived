/* UNIT 0x001C0180 0x001C0768 */
extern int D_004B2840[];

int func_001C0180(void *arg0, int arg1)
{
    switch (arg1) {
    case 0xFAA:
    case 0xFAF:
    case 0xFB4:
    case 0xFB9:
    case 0xFC3:
    case 0xFC8:
    case 0xFCD:
    case 0xFEB:
    case 0xFF0:
    case 0xFF5:
    case 0x1009:
    case 0x1013:
    case 0x1018:
    case 0x101D:
    case 0x1022:
    case 0x103B:
    case 0x1040:
    case 0x104A:
    case 0x104F:
    case 0x1054:
    case 0x1068:
    case 0x106D:
    case 0x1072:
    case 0x1077:
    case 0x107C:
    case 0x1081:
    case 0x1086:
    case 0x108B:
    case 0x1090:
    case 0x1095:
    case 0x109A:
    case 0x109F:
    case 0x10A4:
    case 0x10A9:
    case 0x10B3:
    case 0x10B8:
    case 0x10BD:
    case 0x10C2:
    case 0x10C7:
    case 0x10CC:
    case 0x10D1:
    case 0x10D6:
    case 0x10DB:
    case 0x10EA:
    case 0x10EF:
    case 0x10F4:
        return 0;
    case 0x10AE:
        return D_004B2840[0] == 0x14;
    case 0x1004:
        if (D_004B2840[0] == 0xB || D_004B2840[0] == 0xF || D_004B2840[0] == 0x10)
            return 1;
        return 0;
    case 0x51E:
    case 0x523:
    case 0x528:
    case 0xFA0:
    case 0xFA5:
    case 0x1027:
    case 0x102C:
    case 0x1031:
    case 0x1036:
    case 0x1059:
    case 0x105E:
    case 0x1063:
    case 0x10E0:
    case 0x10E5:
        return 1;
    case 0x100E:
        if (D_004B2840[0] == 5 || D_004B2840[0] == 0xE || D_004B2840[0] == 0x16)
            return 1;
        return 0;
    case 0xFBE:
        if (D_004B2840[0] == 0x14 || D_004B2840[0] == 0x11 || D_004B2840[0] == 0x12)
            return 0;
        return 1;
    case 0xFFF:
        if (D_004B2840[0] == 5 || D_004B2840[0] == 0xE || D_004B2840[0] == 0x16
            || D_004B2840[0] == 0x14 || D_004B2840[0] == 0x11 || D_004B2840[0] == 0x12)
            return 0;
        return 1;
    case 0xFFA:
        if (D_004B2840[0] == 0xB || D_004B2840[0] == 0xF || D_004B2840[0] == 0x10
            || D_004B2840[0] == 0x14 || D_004B2840[0] == 0x11 || D_004B2840[0] == 0x12)
            return 0;
        return 1;
    case 0xFD2:
        if (D_004B2840[0] == 0xB || D_004B2840[0] == 0xF || D_004B2840[0] == 0x10)
            return 0;
        return 1;
    case 0xFE1:
    case 0xFE6:
        return D_004B2840[0] != 0xC;
    default:
        return 1;
    }
}
