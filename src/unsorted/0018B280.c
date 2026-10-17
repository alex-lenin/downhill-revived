/* UNIT 0x0018B280 0x0018B3A8 */
extern float D_003F4860[];
extern int D_003F4780[][14];
extern void func_00203590(void *, int, int, int);
extern void func_00203B68(void *, int, float);
extern void func_002038E0(void *, int);
extern void func_00203CB8(void *);
extern void func_00203DC8(void *, int);
extern void func_00209060(int);

void func_0018B280(void)
{
    int sound[4];
    int i;
    int j;
    int id;
    float *volume;

    volume = D_003F4860;
    id = 0x2352;
    for (i = 0; i < 14; i++) {
        sound[2] = 0;
        func_00203590(sound, id, 0, 5);
        id += 10;
        func_00203B68(sound, 1, *volume++);
        func_002038E0(sound, 0);
        func_00203CB8(sound);
    }
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 14; i++) {
            if (D_003F4780[j][i] != 0) {
                func_00209060(D_003F4780[j][i]);
            }
            D_003F4780[j][i] = 0;
            sound[2] = 0;
            func_00203590(sound, i * 10 + 0x232A, 0, 1);
            func_00203DC8(sound, 0);
        }
    }
}
