/* UNIT 0x00172350 0x00172408 */
typedef struct Obj {
 char pad0[32];
 unsigned int unk20;
 unsigned int unk24;
 unsigned int unk28;
 unsigned int unk2C;
 unsigned int unk30;
 unsigned int unk34;
 unsigned int unk38;
 unsigned int unk3C;
 char pad40[12];
 float unk4C;
 float unk50;
 float unk54;
 float unk58;
 char pad5c[8228];
 int unk2080;
} Obj;
extern void func_00163970(void *,int,void *);
void func_00172350(Obj *arg0) {
    func_00163970(arg0, 0, (char *)arg0 + 0x80);
    arg0->unk4C = 0.75f;
    arg0->unk50 = 0.375f;
    arg0->unk54 = 1.0f;
    arg0->unk58 = 0.0008f;
    arg0->unk20 = 0x20FFFF80;
    arg0->unk24 = 0x20202080;
    arg0->unk28 = 0xFFFF2080;
    arg0->unk2C = 0xFFFF2080;
    arg0->unk30 = 0xFF20FF80;
    arg0->unk34 = 0x2020FF80;
    arg0->unk38 = 0x20FF2080;
    arg0->unk3C = 0x20FF2080;
    arg0->unk2080 = 0;
}
