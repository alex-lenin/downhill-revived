/* UNIT 0x001AF0F8 0x001AF3F0 */
#include "camera.h"
typedef int s32;
typedef float f32;
extern int func_00138560(void *);

extern float func_00236088(float, float, float, float);
extern float D_0029D3E0, D_0029D3E4;
static inline int right(void *p) { if (*(unsigned char *)(p+0x13DA)) return *(int *)(p+0x1264); return *(int *)(p+0x119C); }
static inline int left(void *p) { if (*(unsigned char *)(p+0x13D9)) return *(int *)(p+0x1260); return *(int *)(p+0x1198); }
void func_001AF0F8(void *arg0) {
    float temp_f2, temp_f2_2, temp_f0, temp_f0_2, temp_f0_3, temp_f0_4;
    float var_f3, var_f20;
    int temp_s1;
    void *temp_s2, *input;
    input = *(void **)((char *)arg0 + 0x18);
    if (input) {
        temp_s2 = input + 0x1E40;
        temp_s1 = func_00138560(input + 0x40C0) & 255;
        if (!temp_s1 && right(*(void **)((char *)arg0+0x18)))
            *(float *)(arg0+4)=func_00236088(*(float *)(arg0+4),-1.2f,D_0029D3E0,D_0029D3E0*0.5f);
        else if (!temp_s1 && left(*(void **)((char *)arg0+0x18)))
            *(float *)(arg0+4)=func_00236088(*(float *)(arg0+4),0.6f,D_0029D3E4,D_0029D3E4*0.5f);
        else
            *(float *)(arg0+4)=func_00236088(*(float *)(arg0+4),0.0f,0.025f,0.01f);
        temp_s2 += 0x16F0;
        if (temp_s1 && left(*(void **)((char *)arg0+0x18)))
            *(float *)(arg0+8)=func_00236088(*(float *)(arg0+8),-1.2f,D_0029D3E0,D_0029D3E0*0.5f);
        else if (temp_s1 && right(*(void **)((char *)arg0+0x18)))
            *(float *)(arg0+8)=func_00236088(*(float *)(arg0+8),0.6f,D_0029D3E4,D_0029D3E4*0.5f);
        else
            *(float *)(arg0+8)=func_00236088(*(float *)(arg0+8),0.0f,0.025f,0.01f);
        temp_f2 = (*(f32 *)((char *)arg0 + 0x4));
        if (temp_f2 < 0.0f) {
            temp_f0 = temp_f2 * -0.8333333f;
            var_f3 = -(((temp_f0 * -2.0f) + 3.0f) * temp_f0 * temp_f0);
        } else {
            temp_f0_2 = temp_f2 * 1.6666666f;
            var_f3 = ((temp_f0_2 * -2.0f) + 3.0f) * temp_f0_2 * temp_f0_2;
        }
        temp_f2_2 = (*(f32 *)((char *)arg0 + 0x8));
        if (temp_f2_2 < 0.0f) {
            temp_f0_3 = temp_f2_2 * -0.8333333f;
            var_f20 = -(((temp_f0_3 * -2.0f) + 3.0f) * temp_f0_3 * temp_f0_3);
        } else {
            temp_f0_4 = temp_f2_2 * 1.6666666f;
            var_f20 = ((temp_f0_4 * -2.0f) + 3.0f) * temp_f0_4 * temp_f0_4;
        }
        func_001B3028((RiderCamera *)(temp_s2), 4, var_f3 * (*(f32 *)((char *)arg0 + 0xC)));
        func_001B3028((RiderCamera *)(temp_s2), 5, var_f20);
        (*(f32 *)((char *)arg0 + 0xC)) = 1.0f;
    }
}
