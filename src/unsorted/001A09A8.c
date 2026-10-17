/* UNIT 0x001A09A8 0x001A0FD0 */
/* Rider profile screen (case 0 of func_001A0730).
   D_0026F9D0 holds 18 rider entries (0x78 each); D_004225A0 holds 18
   track entries (0x694 each). names[] holds the 4 difficulty strings
   copied from D_0028E608 ("Novice"/"Sport"/"Expert"/"Pro", 8 bytes each);
   text[] is the sprintf scratch buffer at sp+0x20. */

extern char D_0026F9D0[];
extern char D_0028E608[];
extern char D_0028E628[];
extern char D_0028E638[];
extern char D_0029D1D0[];
extern char D_0029D1D8[];
extern char D_0029D1E0[];
extern char D_0029D1E8[];
extern char D_0029D1F0[];
extern char D_0029D1F8[];
extern char D_0029D200[];
extern char D_0029D208[];
extern char D_0029D210[];
extern char D_0029D218[];
extern char D_0029D220[];
extern char D_0029D228[];
extern char D_0029D230[];
extern char D_0029D238[];
extern char D_0029D240[];
extern char D_0029D248[];
extern char D_0029D250[];
extern char D_0029D258[];
extern char D_0029D260[];
extern char D_004225A0[];

extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F518(int, int, int, int, int);
extern void func_0020F9A8(int, int, int, int, char *);
extern int func_00254D68(char *, char *, ...);
extern int func_00194C80(void *, int);
extern int func_00194E30(void *, int);
extern void func_001A1258(void *, int, int, int, int, int, int, int);
extern void *memcpy(void *, const void *, unsigned int);

void func_001A09A8(void *a0, int a1)
{
    char names[32];
    char text[32];
    char *rider;
    char *track;
    int v0;

    rider = D_0026F9D0 + a1 * 0x78;
    memcpy(names, D_0028E608, 32);
    func_0020FB80(0, 0x120, 0xE0, -0x6B, -0x20);
    func_0020F518(0, 0x6E, 0x6E, 0xC8, 0xAA);
    func_0020F9A8(0, 0, 0xE6, 0x2E, D_0029D1D0);
    func_0020F9A8(0, 0, 0xE6, 0x38, D_0029D1D8);
    func_0020F9A8(0, 0, 0xE6, 0x42, D_0029D1E0);
    func_0020F9A8(0, 0, 0x14, 0x70, D_0029D1E8);
    func_0020F9A8(0, 0, 0xAA, 0x70, D_0029D1F0);
    func_0020F9A8(0, 0, 0xF0, 0x70, D_0029D1F8);
    func_0020F9A8(0, 0, 0x14A, 0x70, D_0029D200);
    func_0020F9A8(0, 0, 0x186, 0x70, D_0029D208);
    func_0020F9A8(0, 0, 0x1C2, 0x70, D_0029D210);
    func_0020F9A8(0, 0, 0x1FE, 0x70, D_0029D218);
    func_0020F9A8(0, 0, 0x14, 0x7B, D_0029D220);
    func_0020F9A8(0, 0, 0x14, 0x85, D_0028E628);
    func_0020F9A8(0, 0, 0x14, 0x8F, D_0029D228);
    func_0020F9A8(0, 0, 0x14, 0x99, D_0029D230);
    func_0020F9A8(0, 0, 0x14, 0xA3, D_0029D238);
    func_0020F9A8(0, 0, 0x14, 0xAD, D_0028E638);
    func_0020F518(0, 0x50, 0x78, 0x50, 0xAA);
    func_00254D68(text, D_0029D240, rider + 0x30, rider + 0x3E);
    func_0020F9A8(0, 0, 0x118, 0x2E, text);
    func_00254D68(text, D_0029D248, rider + 0x5A);
    func_0020F9A8(0, 0, 0x118, 0x38, text);
    func_00254D68(text, D_0029D250, rider + 0x4C);
    func_0020F9A8(0, 0, 0x118, 0x42, text);
    track = D_004225A0 + a1 * 0x694;
    func_00254D68(text, D_0029D258, func_00194C80(track, 3), 0x18);
    func_0020F9A8(0, 0, 0xAA, 0x7B, text);
    func_00254D68(text, D_0029D260, func_00194C80(track, 2), 7);
    func_0020F9A8(0, 0, 0xAA, 0x85, text);
    func_00254D68(text, D_0029D260, func_00194C80(track, 0), 7);
    func_0020F9A8(0, 0, 0xAA, 0x8F, text);
    func_00254D68(text, D_0029D260, func_00194C80(track, 1), 7);
    func_0020F9A8(0, 0, 0xAA, 0x99, text);
    func_00254D68(text, D_0029D260, func_00194C80(track, 4), 0xD);
    func_0020F9A8(0, 0, 0xAA, 0xA3, text);
    func_00254D68(text, D_0029D260, func_00194C80(track, 5), 6);
    func_0020F9A8(0, 0, 0xAA, 0xAD, text);
    v0 = func_00194E30(track, 3);
    func_0020F9A8(0, 0, 0xF0, 0x7B, names + (v0 << 3));
    v0 = func_00194E30(track, 2);
    func_0020F9A8(0, 0, 0xF0, 0x85, names + (v0 << 3));
    v0 = func_00194E30(track, 0);
    func_0020F9A8(0, 0, 0xF0, 0x8F, names + (v0 << 3));
    v0 = func_00194E30(track, 1);
    func_0020F9A8(0, 0, 0xF0, 0x99, names + (v0 << 3));
    v0 = func_00194E30(track, 4);
    func_0020F9A8(0, 0, 0xF0, 0xA3, names + (v0 << 3));
    func_001A1258(a0, 3, a1, 0x7B, 0x14A, 0x186, 0x1C2, 0x1FE);
    func_001A1258(a0, 2, a1, 0x85, 0x14A, 0x186, 0x1C2, 0x1FE);
    func_001A1258(a0, 0, a1, 0x8F, 0x14A, 0x186, 0x1C2, 0x1FE);
    func_001A1258(a0, 1, a1, 0x99, 0x14A, 0x186, 0x1C2, 0x1FE);
    func_001A1258(a0, 4, a1, 0xA3, 0x14A, 0x186, 0x1C2, 0x1FE);
}
