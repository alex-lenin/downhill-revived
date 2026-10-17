/* UNIT 0x00116C48 0x00116FA0 */
typedef struct State { char pad[0x796C]; int x796C; } State;
typedef struct Obj { char pad[0x58]; State *x58; char pad5C[0x1C]; float x78,x7C; } Obj;
extern struct Globals { char pad[0x5BB4]; int x5BB4; char pad5BB8[0x24]; int x5BDC; } D_00398C40;
static inline float maxf(float a,float b) { float r; __asm__ volatile("max.s %0,%1,%2":"=&f"(r):"f"(a),"f"(b)); return r; }
static inline float minf(float a,float b) { float r; __asm__ volatile("min.s %0,%1,%2":"=&f"(r):"f"(a),"f"(b)); return r; }
void func_00116C48(Obj *arg0) {
    float temp_f0;
    float temp_f2;
    float temp_f3;
    float var_f0;
    float var_f1;
    int temp_a1;
    int var_v0;

    temp_a1 = arg0->x58->x796C;
    switch(temp_a1) {
    case 0x20: arg0->x78=0.18f; arg0->x7C=-0.15f; break;
    case 0x40: arg0->x78=0.18f; arg0->x7C=-0.13f; break;
    case 0x60: arg0->x78=0.16f; arg0->x7C=-0.1f; break;
    case 0x80: arg0->x78=0.11f; arg0->x7C=-0.11f; break;
    case 0xA0: arg0->x78=0.13f; arg0->x7C=-0.11f; break;
    case 0xC0: arg0->x78=0.14f; arg0->x7C=-0.13f; break;
    case 0xE0: arg0->x78=0.05f; arg0->x7C=-0.05f; break;
    case 0x100: arg0->x78=0.08f; arg0->x7C=-0.08f; break;
    case 0x140: arg0->x78=0.07f; arg0->x7C=-0.07f; break;
    case 0x160: arg0->x78=0.2f; arg0->x7C=-0.18f; break;
    case 0x180: arg0->x78=0.09f; arg0->x7C=-0.09f; break;
    case 0x1A0: arg0->x78=0.11f; arg0->x7C=-0.09f; break;
    case 0x120: arg0->x78=0.2f; arg0->x7C=-0.15f; break;
    case 0x1C0: arg0->x78=0.2f; arg0->x7C=-0.15f; break;
    case 0x1E0: goto fallback;
    default: fallback: arg0->x78=0.2f; arg0->x7C=-0.15f; break;
    }
    if(D_00398C40.x5BB4==0) var_v0=D_00398C40.x5BDC/2;
    else if(D_00398C40.x5BB4==2) var_v0=D_00398C40.x5BDC/2+50;
    else var_v0=D_00398C40.x5BDC/2+25;
    temp_f2 = arg0->x78;
    temp_f3 = arg0->x7C;
    temp_f0 = 1.0f - ((float) var_v0 * 0.01f);
    temp_f2 = temp_f2 + temp_f2 * temp_f0;
    temp_f3 = temp_f3 + temp_f3 * temp_f0;
    arg0->x78 = temp_f2;
    arg0->x7C = temp_f3;
    arg0->x78 = maxf(arg0->x78,0.07f);
    arg0->x7C = minf(arg0->x7C,-0.07f);
}
