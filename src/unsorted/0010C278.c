/* UNIT 0x0010C278 0x0010C4C0 */
typedef struct Obj { int unk0, unk4; char pad8[0x294-8]; float unk294; char pad298[0x440-0x298]; long unk440; } Obj;
typedef struct Input { char pad0[0x1a0]; int unk1A0, unk1A4, unk1A8; char pad1ac[0x268-0x1ac]; int unk268, unk26C, unk270; char pad274[0x3f3-0x274]; unsigned char unk3F3, unk3F4, unk3F5; } Input;
void func_0010CB70(void *, void *);                    
void func_0010E860(void *);                            
int func_0010E9B0(Obj *, Input *);                              
void func_0010F068(void *, void *);                    
float func_001385A8(int);                             
void func_001C7848(int);                               
void func_001C7A10(int);                               
int func_00222B08(int, int);                            

void func_0010C278(Obj *arg0, Input *arg1) {
    float temp_f1;
    float temp_f20;
    float temp_f20_2;
    int var_a0;
    int var_s2;

    var_s2 = 0;
    if ((arg0->unk440 & (1L << 40)) && (arg0->unk4 != 0)) {
        var_s2 = func_0010E9B0(arg0, arg1);
        if (var_s2 != 0) {
            temp_f20 = func_001385A8(arg0->unk0 + 0x40C0);
            temp_f20_2 = temp_f20 - func_001385A8(arg0->unk4 + 0x40C0);
            if (temp_f20_2 > 0.3f) {
                { int on = 1; if (arg1->unk3F4) { arg1->unk26C = on; } else { arg1->unk1A4 = on; arg1->unk26C = 0; } }
            } else if (temp_f20_2 < 0.1f) {
                { int on = 1; if (arg1->unk3F3) { arg1->unk268 = on; } else { arg1->unk1A0 = on; arg1->unk268 = 0; } }
                if (temp_f20_2 < 0.0f) {
                    { int on = 1; if (arg1->unk3F5) { arg1->unk270 = on; } else { arg1->unk1A8 = on; arg1->unk270 = 0; } }
                    { int on = 1; if (arg1->unk3F3) { arg1->unk268 = on; } else { arg1->unk1A0 = on; arg1->unk268 = 0; } }
                }
            }
        }
        temp_f1 = arg0->unk294;
        if (temp_f1 < 10.0f) {
            if (func_00222B08(0, 1) == 0) {
                var_a0 = arg0->unk0;
            } else {
                var_a0 = 0;
                if (arg0->unk440 & (1L << 40)) {
                    var_a0 = arg0->unk4;
                }
            }
            func_001C7848(var_a0 + 0x7720);
        } else if (temp_f1 < 20.0f) {
            func_001C7A10(arg0->unk0 + 0x7720);
        }
    }
    if (var_s2 == 0) {
        if (arg0->unk440 & (1L << 41)) {
            func_0010E860(arg0);
            arg0->unk440 = (long) (arg0->unk440 & ~(1L << 41));
        }
        func_0010F068(arg0, arg1);
        func_0010CB70(arg0, arg1);
    }
}
