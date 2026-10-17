/* UNIT 0x001B3BE0 0x001B3D58 */
struct Part { float x0[18]; float x48; int x4C; }; // 0x50, matches Settings in 001B2E80
struct State { float value, delta, base, lower, upper; char pad[12]; }; // 0x20 = 8 floats
struct Obj { char pad0[0x120]; Part parts[13]; State states[3]; };
extern "C" float D_002702F0[13][13];
extern "C" void func_001B2E80(void *, void *, float);
void func_001B3BE0(Obj *obj)
{
    int i, j;
    // Bases for lower/base/delta; indexed by i*8 (State stride in floats).
    float *pLower = &obj->states[0].lower;
    float *pBase = &obj->states[0].base;
    float *pDelta = &obj->states[0].delta;
    for (i = 0; i < 13; i++) {
        float product = 1.0f;
        float *weight = D_002702F0[i];
        Part *other = obj->parts;
        j = 12;
        do {
            product *= ((1.0f - __builtin_fabsf(other->x48) * *weight) >? 0.0f);
            other++;
            weight++;
        } while (--j >= 0);
        func_001B2E80(&obj->parts[i], obj, product);
    }
    for (i = 0; i < 3; i++) {
        obj->states[i].value = pDelta[i * 8] + pBase[i * 8];
        obj->states[i].value = (obj->states[i].value <? (obj->states[i].upper + pBase[i * 8])) >? pLower[i * 8];
        pDelta[i * 8] = 0.0f;
    }
}
