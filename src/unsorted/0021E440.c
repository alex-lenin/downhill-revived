/* UNIT 0x0021E440 0x0021E4E0 */
typedef struct { char pad[16]; float x10,x14,x18; } Pos;
typedef struct { char pad[16]; float x10,x14,x18; int x1C; Pos *x20; char pad24[8]; float x2C,x30; char pad34[12]; } Entry;
extern Entry D_0062F880[];
extern float D_00660180[];
void func_0021E440(float *out, int index) {
    Entry *entry = &D_0062F880[index];
    float scale = entry->x2C * entry->x30;
    out[0] = D_00660180[0]*scale + entry->x20->x10 + entry->x10;
    out[1] = D_00660180[1]*scale + entry->x20->x14 + entry->x14;
    out[2] = D_00660180[2]*scale + entry->x20->x18 + entry->x18;
    out[0] = -out[0];
    out[1] = -out[1];
    out[2] = -out[2];
}
