/* UNIT 0x002398C0 0x002399F8 */
typedef struct Key { float value,time; } Key;
extern int D_0027EC68[];
extern float D_0027EC78[];
extern Key D_0027EC88[];
extern float D_0075A690[],D_0075A6A0[];
extern int func_00238F60(void);
void func_002398C0(unsigned int index)
{
    int *state;
    float *time;
    int current,next;
    float amount,start,value,squared,span;
    if (index < 4) {
        state = &D_0027EC68[index];
        current = *state;
        if (current < 3) {
            time = &D_0075A690[index];
            span = D_0027EC88[current+1].time - D_0027EC88[current].time;
            start = D_0027EC88[current].time;
            amount = (*time - start) / span;
            value = D_0027EC88[current].value;
            amount = value + (D_0027EC88[current+1].value - value) * amount;
            squared = amount * amount;
            D_0075A6A0[index] = D_0027EC78[index] * ((squared + squared) - squared * squared);
            *time += (float)func_00238F60() * 0.016666668f;
            next = *state + 1;
            if (D_0027EC88[next].time <= *time) *state = next;
        }
    }
}
