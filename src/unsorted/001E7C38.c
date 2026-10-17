/* UNIT 0x001E7C38 0x001E7C78 */
typedef struct {
    int x0;
    int index;
} Entry8;

typedef struct {
    char pad0[0x4C];
    float x4C;
    char pad50[0x4];
} Entry54;

extern Entry8 D_002752B0[];
extern Entry54 D_00273D60[];

float func_001E7C38(int arg0, int arg1)
{
    Entry54 *e = &D_00273D60[D_002752B0[arg1].index];

    return e->x4C * (1.0f / 60.0f);
}
