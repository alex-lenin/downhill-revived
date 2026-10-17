/* UNIT 0x00176A80 0x00176E40 */
struct Part { int x0; unsigned char x4; };
struct Obj {
 char pad0[0xF7C];
 int xF7C;
 int xF80;
 char padF84[0x1E4];
 struct Part * x1168;
 struct Part * x116C;
 struct Part * x1170;
 char pad1174[0x4];
 struct Part * x1178;
 struct Part * x117C;
 struct Part * x1180;
 char pad1184[0x4];
 struct Part * x1188;
 char pad118C[0x4];
 float x1190[4];
 float x11A0[4];
 float x11B0[4];
 float x11C0[4];
 char pad11D0[0x10];
 float x11E0[4];
 float x11F0[4];
 float x1200[4];
 float x1210[4];
 char pad1220[0x5C];
 int x127C;
 int x1280;
 char pad1284[0x8C];
 int x1310;
 char pad1314[0x4D];
 unsigned char x1361;
};
extern struct Settings { char pad[0x10C]; int x10C; char p110[0x178]; int x288; } D_004B25A0;
extern int D_0043F348[];
extern int func_0023DED0(void);
extern void func_00176930(struct Obj *,struct Part *,float *,float,float,float);
void func_00176A80(struct Obj *arg0) {
    struct Part *temp_a1_2;
    struct Part *temp_a1_3;
    struct Part *temp_a1_4;
    struct Part *temp_a1;
    int state;

    if (arg0->x127C != 0) {
        if (D_004B25A0.x288 >= 3) {
            temp_a1 = arg0->x1188;
            if (temp_a1 != 0) {
                func_00176930(arg0, temp_a1, arg0->x11A0, 1.2300000190734863f, 1.2000000476837158f, 0.4000000059604645f);
            }
        } else if (func_0023DED0() == 2) {
            state=D_0043F348[0];
            if((unsigned int)(state-2)<3) goto accepted1;
            if(state!=1) goto done1;
            accepted1: {
                func_00176930(arg0, arg0->x1168, arg0->x1190, 1.2300000190734863f, 1.2000000476837158f, 0.5099999904632568f);
                if (arg0->x1280 == 0) {
                    temp_a1_2 = arg0->x1180;
                    if (temp_a1_2 != 0) {
                        func_00176930(arg0, temp_a1_2, arg0->x1210, 1.0f, 1.0f, 1.0f);
                        arg0->x1180->x0 = arg0->xF80;
                    }
                }
                if ((D_004B25A0.x10C != 0xF) && (D_004B25A0.x10C != 0x11)) {
                    func_00176930(arg0, arg0->x1178, arg0->x11E0, 1.4500000476837158f, 1.399999976158142f, 0.4000000059604645f);
                }
                func_00176930(arg0, arg0->x1170, arg0->x11C0, 1.0800000429153442f, 1.100000023841858f, -0.11999999731779099f);
                if (arg0->x1310 > 0) {
                    func_00176930(arg0, arg0->x117C, arg0->x11F0, 1.149999976158142f, 1.399999976158142f, 0.6000000238418579f);
                }
            }
            done1:
            temp_a1_3 = arg0->x116C;
            if ((temp_a1_3 != 0) && (temp_a1_3->x4 != 0)) {
                func_00176930(arg0, temp_a1_3, arg0->x11B0, 0.5f, 1.5f, 1.0f);
            }
        } else {
            state=D_0043F348[0];
            if((unsigned int)(state-2)<3) goto accepted2;
            if(state!=1) goto done2;
            accepted2: {
                func_00176930(arg0, arg0->x1168, arg0->x1190, 0.44999998807907104f, 0.9399999976158142f, 1.2599999904632568f);
                func_00176930(arg0, arg0->x1170, arg0->x11C0, 0.6100000143051147f, 0.8999999761581421f, 1.5f);
                if (D_004B25A0.x10C != 0xF) {
                    if ((D_004B25A0.x10C != 0x11) && (arg0->x1361 != 0)) {
                        func_00176930(arg0, arg0->x1178, arg0->x11E0, 0.5199999809265137f, 1.0f, 1.3980000019073486f);
                    }
                }
                if ((D_004B25A0.x288 >= 2) && (arg0->x1280 == 0)) {
                    arg0->x1180->x0 = arg0->xF7C;
                    func_00176930(arg0, arg0->x1180, arg0->x1200, 0.5f, 1.0f, 1.5f);
                }
                if (arg0->x1310 > 0) {
                    func_00176930(arg0, arg0->x117C, arg0->x11F0, 0.5f, 1.0f, 1.0f);
                }
            }
            done2:
            temp_a1_4 = arg0->x116C;
            if ((temp_a1_4 != 0) && (temp_a1_4->x4 != 0)) {
                func_00176930(arg0, temp_a1_4, arg0->x11B0, 0.5f, 1.0f, 1.2000000476837158f);
            }
        }
    }
}
